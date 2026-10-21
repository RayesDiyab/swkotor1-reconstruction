#include "2e9f118037-movie_list.h"
#include "ef67052670-intro_queue.h"        // client_intro::CountStrings_004b2640
#include "c534281817-startup_string.h"
#include "c7e191db10-native_strings.h"     // WindowCopy005e5b00 / WindowCStr005e5670 (M40 window-native-strings.obj)
#include "7772bffc2f-alias_path.h"         // alias_path::AliasListView::GetAliasPath (M40 m40-alias-lookup.obj)
#include "7b8c81fcff-allocator_live.h"     // live_operator_new / live_free (006FA7E6 / 006FA390)
#include "66a5366142-menu_targets.h"       // kotor_menu20::ClientApp::get_options_005ed700 (M40)
#include "369787b919-window_shared_cells.h"// manager_007a39fc
#include <cstdint>
#include <cstring>
#include <new>
// ---- existing owners: exact declarations --------------------------------------
namespace tlk_file { struct File; }
namespace file_internal {   // M40 m39-constructors.obj
tlk_file::File* __fastcall exofile_construct_005e68d0(tlk_file::File*,void*,kotor_reconstruction::StartupString*,std::uint32_t,kotor_reconstruction::StartupString*);
}
extern "C" std::uint32_t __fastcall file_opened_005e6a10(tlk_file::File*,void*);   // M40 resource-successor file_destroy unit
extern "C" void __fastcall file_destroy_005e69e0(tlk_file::File*,void*);           // M40
namespace path_resolution {  // M40 m40-path.obj
kotor_reconstruction::StartupString* __fastcall resolve_005e68a0(alias_path::AliasListView*,void*,kotor_reconstruction::StartupString*,kotor_reconstruction::StartupString*,std::uint32_t);
}
namespace base_ini_reconstruction {  // M40 ini-string-transforms.obj
kotor_reconstruction::StartupString* __fastcall right_005e6170(kotor_reconstruction::StartupString*,void*,kotor_reconstruction::StartupString*,int);
kotor_reconstruction::StartupString* __fastcall left_005e5f70(kotor_reconstruction::StartupString*,void*,kotor_reconstruction::StartupString*,int);
}
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } } // M40 modern-game-storage.obj
namespace f200_ports { struct Enumeration; }
namespace directory_list {   // lane-resource-lowers directory_list.cpp (CExoArrayList<CExoString>::Add, by value)
void __fastcall Add004059a0(f200_ports::Enumeration*,void*,kotor_reconstruction::StartupString);
}
extern "C" std::int32_t __fastcall exostring_compare_no_case_005e6450(kotor_reconstruction::StartupString*,void*,kotor_reconstruction::StartupString*); // lane-resource-lowers
extern "C" void* kotor_cell_rules_007a3a28;                       // lane-prepare owner (.data CSWRules*)
extern "C" const unsigned char kotor_rdata_live_count_0073ecd2;   // lane-tlk owner (.rdata 07h)

namespace client_movies {
namespace {
using Canonical=kotor_reconstruction::StartupString;
Canonical* C(String* s){return std::launder(static_cast<Canonical*>(static_cast<void*>(s)));}
void* P(const void* p,std::size_t o){void* v;std::memcpy(&v,static_cast<const U8*>(p)+o,4);return v;}
I32 W(const void* p,std::size_t o){I32 v;std::memcpy(&v,static_cast<const U8*>(p)+o,4);return v;}
const char* Text(String* s){return WindowCStr005e5670(C(s),nullptr);}
// Pinned PE literals.
const char kEmpty[]="";                         // 0073D71C
const char kColon[]=":";                        // 0073E2A0
const char kMoviesFormat[]="MOVIES:%s";         // 0073D7D8
const char kReadBinary[]="rb";                  // 0073D5B0
const char kLiveFormat[]="LIVE%d";              // 007453C0
const char kLiveMovieFormat[]="LIVE%d:movies\\%s"; // 0074EC3C
constexpr std::uint32_t kBikType=0x80f;         // 005FBD03/005FBDF3/005FBE7D
// 005FBCCF..005FBD3C / 005FBDC7..005FBE2A: new(4), CExoFile(path,0x80F,"rb");
// the "rb" temporary is constructed and destroyed only when new succeeded.
tlk_file::File* OpenMovie(String* path) {
    auto& o=options_video::options_bindings;
    void* const memory=live_operator_new(4);
    if(memory==nullptr)return nullptr;
    String mode;o.text_string_005e5a90(&mode,nullptr,kReadBinary);
    tlk_file::File* const file=file_internal::exofile_construct_005e68d0(static_cast<tlk_file::File*>(memory),nullptr,C(path),kBikType,C(&mode));
    o.destroy_string_005e5c20(&mode,nullptr);
    return file;
}
void CloseMovie(tlk_file::File* file){file_destroy_005e69e0(file,nullptr);live_free(file);} // 005FBD68/005FBD6E, 005FBE5A/005FBE60
}

// 005E5390 CExoString::operator!=(char const*): ECX=this, RET 4. Null string and
// null/empty text compare as empty.
I32 __fastcall StringNotEqual_005e5390(String* self,void* unused_edx,const char* text) {
    (void)unused_edx;
    const char* const data=self->text;                                  // 005E5390
    if(data==nullptr)return (text!=nullptr && text[0]!=0)?1:0;          // 005E53F2..005E53FD
    if(text==nullptr)return data[0]!=0?1:0;                             // 005E53E7..005E53EA
    for(std::size_t i=0;;++i) {                                         // 005E53A0..005E53C2 bytewise
        if(data[i]!=text[i])return 1;                                   // 005E53D4..005E53DD
        if(data[i]==0)return 0;                                         // 005E53C4
    }
}

// 005E5EB0 CExoString::Find(CExoString const&,int): naive scan, RET 8, -1 if absent.
I32 __fastcall StringFind_005e5eb0(String* self,void* unused_edx,String* needle,I32 start) {
    (void)unused_edx;
    const char* const base=self->text;const char* const sub=needle->text;
    if(base==nullptr||sub==nullptr||start<0)return -1;                  // 005E5EB4..005E5EC8
    const char* at=base;
    for(I32 i=0;i<start;++i){if(*at==0)return -1;++at;}                 // 005E5ECA..005E5EDB
    I32 k=0;
    if(*at!=0) {                                                        // 005E5EDD..005E5EE3
        for(;;) {
            const char want=sub[k];                                     // 005E5EE5
            if(want==0)return static_cast<I32>(at-base);                // 005E5EEA -> 005E5F03
            if(at[k]==want)++k;else{++at;k=0;}                          // 005E5EEC..005E5EF6
            if(at[k]==0)break;                                          // 005E5EF7..005E5EFB
        }
    }
    return sub[k]==0?static_cast<I32>(at-base):-1;                      // 005E5EFD..005E5F0C
}

// 005F7C10 CExoArrayList<CExoString>::AddUnique(CExoString): RET 8, destroys value.
void __fastcall AddUnique_005f7c10(void* list,void* unused_edx,String value) {
    (void)unused_edx;
    String probe;                                                       // 005F7C29 outgoing by-value area
    WindowCopy005e5b00(C(&probe),nullptr,C(&value));                    // 005F7C3F
    if(client_intro::CountStrings_004b2640(list,nullptr,probe)==0)      // 005F7C46..005F7C4D (count destroys probe)
        directory_list::Add004059a0(static_cast<f200_ports::Enumeration*>(list),nullptr,*C(&value)); // 005F7C4F..005F7C64 (005E5B00 copy, Add destroys)
    options_video::options_bindings.destroy_string_005e5c20(&value,nullptr); // 005F7C69..005F7C75
}

// 004138B0 C2DA::GetRowIndex(CExoString const&): RET 4, -1 if absent.
I32 __fastcall C2daGetRowIndex_004138b0(void* table,void* unused_edx,String* label) {
    (void)unused_edx;
    if(W(table,0x3c)!=0) {                                              // 004138B4..004138BF packed label table
        for(I32 i=0;i<W(table,0x24);++i) {                              // 004138C1..004138F7 (rows reread)
            const char* const base=static_cast<const char*>(P(table,0x48));
            std::uint16_t offset;std::memcpy(&offset,static_cast<const U8*>(P(table,0x4c))+static_cast<std::size_t>(i)*2,2); // 004138DB/004138DE
            if(_stricmp(base+offset,Text(label))==0)return i;          // 004138D2..004138EF __stricmp 0070ACAF
        }
        return -1;                                                      // 004138F9
    }
    for(I32 i=0;i<W(table,0x24);++i) {                                  // 00413902..00413926
        Canonical* const entry=std::launder(static_cast<Canonical*>(static_cast<void*>(static_cast<U8*>(P(table,0x30))+static_cast<std::size_t>(i)*8)));
        if(exostring_compare_no_case_005e6450(entry,nullptr,C(label))!=0)return i; // 00413910..0041391E
    }
    return -1;                                                          // 00413928
}

// 0061D230 CClientOptions::SetMovieShown(CExoString const&,int): RET 8.
void __fastcall SetMovieShown_0061d230(void* options,void* unused_edx,String* name,I32 shown) {
    (void)unused_edx;auto& o=options_video::options_bindings;
    void* const movies=P(P(kotor_cell_rules_007a3a28,0xb8),0x11c);      // 0061D248..0061D253
    I32 row=C2daGetRowIndex_004138b0(movies,nullptr,name);              // 0061D269
    if(row<0) {                                                         // 0061D270/0061D272
        const char* const dot=std::strrchr(Text(name),'.');             // 0061D278..0061D282
        const char* const slash=std::strrchr(Text(name),'\\');          // 0061D28A..0061D296
        if(dot==nullptr||slash==nullptr)return;                         // 0061D29E..0061D2AA
        const I32 length=static_cast<I32>(std::strlen(Text(name)));    // 0061D2B1..0061D2CB
        const I32 dot_end=static_cast<I32>(dot-Text(name))+1;           // 0061D2CD..0061D2D6
        const I32 slash_end=static_cast<I32>(slash-Text(name))+1;       // 0061D2D7..0061D2DE
        String stem,right;                                              // [esp+14h], [esp+1Ch]
        base_ini_reconstruction::right_005e6170(C(name),nullptr,C(&stem),length-slash_end); // 0061D2E1..0061D2E9 hidden out
        Canonical* const left=base_ini_reconstruction::left_005e5f70(C(&stem),nullptr,C(&right),dot_end-slash_end-1); // 0061D2EE..0061D303
        C(&stem)->Assign005e5c50(left);                                 // 0061D308..0061D312
        o.destroy_string_005e5c20(&right,nullptr);                      // 0061D317..0061D320
        void* const again=P(P(kotor_cell_rules_007a3a28,0xb8),0x11c);   // 0061D325..0061D330 reloaded
        row=static_cast<I32>(static_cast<U8>(C2daGetRowIndex_004138b0(again,nullptr,&stem))); // 0061D33D/0061D342 MOVZX AL
        o.destroy_string_005e5c20(&stem,nullptr);                       // 0061D348..0061D36F (row>=0 always)
    }
    const U32 index=static_cast<U32>(row)>>3;                           // 0061D378/0061D37A
    if(index>=0xb)return;                                               // 0061D37D unsigned JAE
    const unsigned bit=static_cast<unsigned>(row)-index*8u;             // 0061D386..0061D38D
    U8* const shown_bits=static_cast<U8*>(options)+0x60+index;          // [eax+edi+60h]
    const U8 mask=static_cast<U8>(1u<<bit);                             // 0061D391/0061D397 (DL shl CL)
    if(shown!=0)*shown_bits=static_cast<U8>(*shown_bits|mask);         // 0061D38F..0061D39D
    else *shown_bits=static_cast<U8>(*shown_bits&static_cast<U8>(~mask)); // 0061D3A1..0061D3A9
    (void)SaveOptionsApp_005edf70(P(kotor_menu_window_cells::manager_007a39fc,4),nullptr); // 0061D3AF..0061D3B7
}

// 005EDF70 CClientExoApp::SaveOptions: mov ecx,[ecx+4]; jmp 005F46F0.
I32 __fastcall SaveOptionsApp_005edf70(void* app,void* unused_edx){(void)unused_edx;return SaveOptionsInternal_005f46f0(P(app,4),nullptr);}
// 005F46F0 CClientExoAppInternal::SaveOptions: ECX unused; options from the app manager.
I32 __fastcall SaveOptionsInternal_005f46f0(void* internal,void* unused_edx) {
    (void)internal;(void)unused_edx;
    auto* const app=static_cast<kotor_menu20::ClientApp*>(P(kotor_menu_window_cells::manager_007a39fc,4)); // 005F46F0/005F46F5
    return SaveOptions_0061b780(app->get_options_005ed700(),nullptr);  // 005F46F8..005F46FF tail jump
}

// 005FBBF0 CClientExoAppInternal::AddMovieToExoArrayList(CExoString const&, CExoArrayList<CExoString>*): RET 8.
I32 __fastcall AddMovieToExoArrayList_005fbbf0(Client* self,void* unused_edx,String* name,void* list) {
    (void)unused_edx;auto& o=options_video::options_bindings;
    I32 found=0;                                                        // [esp+0Ch] = 0 at 005FBC21
    if(StringNotEqual_005e5390(name,nullptr,kEmpty)==0)return 0;        // 005FBC1A..005FBC2C -> 005FBF33
    {
        String colon;o.text_string_005e5a90(&colon,nullptr,kColon);     // 005FBC32..005FBC3B
        const I32 position=StringFind_005e5eb0(name,nullptr,&colon,0);  // 005FBC40..005FBC4C
        o.destroy_string_005e5c20(&colon,nullptr);                      // 005FBC51..005FBC5F
        if(position!=-1) {                                              // 005FBC64/005FBC67
            String copy;WindowCopy005e5b00(C(&copy),nullptr,C(name));  // 005FBC69..005FBC73 by-value argument
            AddUnique_005f7c10(list,nullptr,copy);                      // 005FBC78/005FBC7B
            return 1;                                                   // 005FBC80
        }
    }
    String path,alias;                                                  // [esp+20h], [esp+28h]
    o.default_string_005b3190(&path,nullptr);                           // 005FBC99..005FBC9D
    o.default_string_005b3190(&alias,nullptr);                          // 005FBCA2..005FBCAE
    o.format_005e5680(&path,kMoviesFormat,Text(name));                  // 005FBCB3..005FBCCA
    tlk_file::File* file=OpenMovie(&path);                              // 005FBCCF..005FBD3C
    if(file_opened_005e6a10(file,nullptr)!=0) {                         // 005FBD41..005FBD4A (also when file==null, as original)
        found=1;                                                        // 005FBE4C
    } else {
        I32 live=1;                                                     // 005FBD57
        if(kotor_rdata_live_count_0073ecd2>1) {                         // 005FBD50/005FBD5C unsigned JBE
            do {
                if(file!=nullptr){CloseMovie(file);file=nullptr;}       // 005FBD62..005FBD76
                o.format_005e5680(&alias,kLiveFormat,live);             // 005FBD78..005FBD83
                auto* const aliases=static_cast<alias_path::AliasListView*>(P(kotor_menu_10_19::dependencies::base_007a39e0,0xc)); // 005FBD88..005FBD8D
                if(aliases->GetAliasPath(C(&alias),0)->BorrowedLength005e5790()!=0) { // 005FBD93..005FBDA8
                    o.format_005e5680(&path,kLiveMovieFormat,live,Text(name)); // 005FBDAE..005FBDC2
                    file=OpenMovie(&path);                              // 005FBDC7..005FBE2A
                    if(file_opened_005e6a10(file,nullptr)!=0){found=1;break;} // 005FBE2F..005FBE38 -> 005FBE4C
                }
                ++live;                                                 // 005FBE41
            } while(live<static_cast<I32>(kotor_rdata_live_count_0073ecd2)); // 005FBE3A..005FBE44
        }
    }
    if(file!=nullptr)CloseMovie(file);                                  // 005FBE54..005FBE65
    if(found==0) {                                                      // 005FBE68..005FBE6E
        o.destroy_string_005e5c20(&alias,nullptr);                      // 005FBF14..005FBF1D
        o.destroy_string_005e5c20(&path,nullptr);                       // 005FBF22..005FBF2E
        return 0;                                                       // 005FBF39
    }
    {
        auto* const aliases=static_cast<alias_path::AliasListView*>(P(kotor_menu_10_19::dependencies::base_007a39e0,0xc)); // 005FBE74..005FBE7A
        String resolved;                                                // [esp+30h] hidden return
        Canonical* const result=path_resolution::resolve_005e68a0(aliases,nullptr,C(&resolved),C(&path),kBikType); // 005FBE7D..005FBE8C
        C(&path)->Assign005e5c50(result);                               // 005FBE91..005FBE9B
        o.destroy_string_005e5c20(&resolved,nullptr);                   // 005FBEA0..005FBEA9
    }
    String copy;WindowCopy005e5b00(C(&copy),nullptr,C(&path));          // 005FBEAE..005FBEBC by-value argument
    AddUnique_005f7c10(list,nullptr,copy);                              // 005FBEC1/005FBEC4
    SetMovieShown_0061d230(P(self,4),nullptr,&path,1);                  // 005FBEC9..005FBED7
    o.destroy_string_005e5c20(&alias,nullptr);                          // 005FBEDC..005FBEE5
    o.destroy_string_005e5c20(&path,nullptr);                           // 005FBEEA..005FBEF6
    return 1;                                                           // 005FBEFB
}
}
