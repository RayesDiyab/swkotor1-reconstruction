// lane-tlk: CTlkTable::ParseStr 0041DD30, the CTlkTable / CSWTlkTable typed vtables
// (D1 slot form, D21 typed constant data, D41 typed reads), the CTlkTable slot body
// 005B5E90, and CRT strncmp 006FAFF0.
// Evidence: dumpbin /disasm swkotor.exe 0x41DD30-0x41E0E5 (dis/c_0x41D800.txt), 0x5B5E90,
// 0x6FAFF0-0x6FB028; vtable words 0073ECDC={005B5E90}, 0074E700={00674B90} (installed by
// 0041D8E3 and 005F899B); literals 0073ECEC "<UNRECOGNIZED TOKEN>", 0073ECE4 "CUSTOM".
#include "6db30bdc1c-tlk_core.h"
#include "c7e191db10-native_strings.h"     // WindowCStr005e5670 (005E5670)
#include "689940810d-string_priority.h"    // GuiAssign005e5140 (005E5140), GuiConcat005e5d10 (005E5D10)
#include <cstdlib>
#include <new>

// ---- 006FAFF0 strncmp (CRT asm): repne scasb bounds the compare by strlen(first)+1 and count,
// repe cmpsb compares, result -1/0/+1 from the last compared unsigned bytes.
extern "C" __declspec(noinline) int __cdecl kotor_strncmp_006faff0(const char* first,const char* second,tlk_core::Word count) {
    if(!count)return 0;                                        // 006FAFF9 jecxz -> eax=0
    tlk_core::Word scanned=0;                                  // 006FB004 repne scasb over first
    while(scanned<count){const char c=first[scanned++];if(!c)break;}
    tlk_core::Word k=0;unsigned char s2,s1;                    // 006FB00F repe cmpsb (esi=second, edi=first)
    do{s2=static_cast<unsigned char>(second[k]);s1=static_cast<unsigned char>(first[k]);++k;}while(k<scanned&&s2==s1);
    if(s2>s1)return -1;                                        // 006FB019 ja -> not 0
    if(s2==s1)return 0;                                        // 006FB01B je
    return 1;                                                  // 006FB01D sub 2; not
}

namespace tlk_core {
// ---- 005B5E90 `ret 8`: the CTlkTable slot0 body (no effect on token or value).
void __fastcall TokenValueBase005b5e90(Tlk*,void*,Token*,String*) {}
}

// ---- typed vtables (one slot each; the following words are unrelated literals)
extern "C" const tlk_core::TlkMethods kotor_vtable_ctlktable_0073ecdc={&tlk_core::TokenValueBase005b5e90};
extern "C" const tlk_core::TlkMethods kotor_vtable_cswtlktable_0074e700={&tlk_core::SwTokenValue00674b90};

namespace tlk_core {
const TlkMethods* MethodsOf(const Tlk* self) {
    const Word vptr=get<Word>(self,0);
    // Typed table addresses, or the original vptr values the M40 sources store as tokens
    // (service-retained-11 0041D8D0 writes 0073ECDCh; start-body 005F899B writes 0074E700h).
    if(vptr==reinterpret_cast<Word>(&kotor_vtable_ctlktable_0073ecdc)||vptr==0x0073ecdcu)return &kotor_vtable_ctlktable_0073ecdc;
    if(vptr==reinterpret_cast<Word>(&kotor_vtable_cswtlktable_0074e700)||vptr==0x0074e700u)return &kotor_vtable_cswtlktable_0074e700;
    std::abort();                                              // unknown receiver class: fail closed
}
}

namespace {
using namespace tlk_core;
const char unrecognized_0073ecec[]="<UNRECOGNIZED TOKEN>";      // 0041DE57 push 73ECECh
const char custom_0073ece4[]="CUSTOM";                         // 0041DEA5 mov edi,73ECE4h (3 words compared)
// CTlkTable +48h custom-token row, stride 0Ch (0041DB95 lea ecx,[eax+eax*2]; *4): {id, CExoString}.
struct CustomToken {Word id;String text;};
static_assert(sizeof(CustomToken)==12&&offsetof(CustomToken,text)==4,"observed custom token row");
// result = result + piece: 005E5D10 into an unconstructed return slot, 005E5C50, then 005E5C20 of the slot.
void append(String& result,String* piece){
    alignas(String) unsigned char slot[sizeof(String)];
    String* sum=GuiConcat005e5d10(&result,nullptr,reinterpret_cast<String*>(slot),piece);
    result.Assign005e5c50(sum);
    sum->~StartupString();
}
// result += CExoString(text, length) (005E5B70); the concat slot is destroyed before the piece.
void append_span(String& result,const char* text,std::int32_t length){
    alignas(String) unsigned char storage[sizeof(String)];
    String* piece=resman_string_counted_005e5b70(reinterpret_cast<String*>(storage),nullptr,text,length);
    append(result,piece);
    piece->~StartupString();
}
}

namespace tlk_core {
void __fastcall ParseStr0041dd30(Tlk* self,void*,String* input) {
    String result;                                             // [esp+28h] 0041DD4E (EH state 0)
    String value;                                              // [esp+30h] 0041DD5D (EH state 1)
    put<Word>(self,0x50,0);                                    // 0041DD73
    const char* const s=WindowCStr005e5670(input,nullptr);     // 0041DD6A..0041DD7B (ebx, not reloaded)
    std::int32_t i=0;                                          // ebp
    std::int32_t start=0;                                      // esi
    if(s[0]){                                                  // 0041DD7D..0041DD87
        do{
            if(s[i]=='<'){                                     // 0041DD90
                if(s[i+1]=='<'){                               // 0041DD9A "<<" escape
                    ++i;                                       // 0041DDA4
                    if(get<Word>(self,0x50)==0)append_span(result,s+start,i-start); // 0041DDA5..0041DDF5 (keeps one '<')
                    start=i+1;                                 // 0041DDFA
                }else{
                    if(get<Word>(self,0x50)==0)append_span(result,s+start,i-start); // 0041DE02..0041DE52
                    (void)GuiAssign005e5140(&value,nullptr,unrecognized_0073ecec); // 0041DE57..0041DE60
                    const std::int32_t first=i+1;              // 0041DE65 edx
                    i=first;                                   // 0041DE68
                    bool closed=false;
                    if(s[i]){                                  // 0041DE6A..0041DE6F
                        for(;;){
                            if(s[i]=='>'){closed=true;break;}  // 0041DE75..0041DE77
                            const char next=s[i+1];++i;        // 0041DE79..0041DE7D
                            if(!next)break;                    // 0041DE80..0041DE82
                        }
                    }
                    if(closed&&first<i){                       // 0041DE87..0041DE89 jge (signed)
                        const Word length=static_cast<Word>(i-first); // 0041DE8F..0041DE96 [esp+1Ch]
                        if(length>=7u&&std::memcmp(s+first,custom_0073ece4,6)==0){ // 0041DE93..0041DEB2 repe cmpsw x3
                            // <CUSTOMn>: decimal digits from the last character back (0041DEB8..0041DEFD).
                            Word number=0,scale=1;bool valid=true;
                            for(std::int32_t k=i-1;k>=first+6;--k){ // 0041DEB8..0041DEC7 jl / 0041DEF6 jge
                                const char d=s[k];             // 0041DED0
                                if(d<'0'||d>'9'){valid=false;break;} // 0041DED3..0041DEDF -> 0041E008
                                number+=static_cast<Word>(d-'0')*scale; // 0041DEE5..0041DEF1
                                scale*=10u;                    // 0041DEEE..0041DEF3
                                if(k-1<first+6&&number==0xffffffffu)valid=false; // 0041DEFA..0041DEFD (after the loop only)
                            }
                            const Word count=get<Word>(self,0x4c);         // 0041DF07
                            if(valid&&count){                  // 0041DF0C..0041DF0E
                                const Word rows=get<Word>(self,0x48);      // 0041DF16 [ecx+48h]
                                Word low=0,high=count;
                                do{                            // 0041DF20..0041DF3A (unsigned)
                                    const Word mid=(high+low)>>1;
                                    const CustomToken* row=reinterpret_cast<const CustomToken*>(rows+mid*12u);
                                    if(row->id>number)high=mid;
                                    else if(row->id<number)low=mid+1;
                                    else{value.Assign005e5c50(&reinterpret_cast<CustomToken*>(rows+mid*12u)->text);break;} // 0041DF45..0041DF51
                                }while(high>low);
                            }
                        }else{
                            // Table token: CRC hash, binary search on +40h rows, exact length and strncmp.
                            const char* const name=s+first;    // 0041DF63..0041DF67 [esp+24h]
                            const Word hash=Hash00410560(name,length); // 0041DF65..0041DF6B [esp+20h]
                            const Word count=get<Word>(self,0x44);     // 0041DF70..0041DF74
                            if(count){
                                const Word rows=get<Word>(self,0x40);  // 0041DF8A..0041DF8D [esp+18h] (held)
                                Word low=0,high=count;
                                do{                            // 0041DF91..0041DFB3 (unsigned)
                                    const Word mid=(high+low)>>1;
                                    const Token* row=reinterpret_cast<const Token*>(rows+mid*0x24u);
                                    if(row->hash>hash)high=mid;
                                    else if(row->hash<hash)low=mid+1;
                                    else{
                                        const Word at=mid*0x24u; // 0041DFB7..0041DFBE esi
                                        if(static_cast<Word>(reinterpret_cast<Token*>(rows+at)->text.BorrowedLength005e5790())==length){ // 0041DFC1..0041DFD0
                                            Token* reread=reinterpret_cast<Token*>(get<Word>(self,0x40)+at); // 0041DFDB [eax+40h]
                                            if(kotor_strncmp_006faff0(WindowCStr005e5670(&reread->text,nullptr),name,length)==0){ // 0041DFDA..0041DFF3
                                                Token* token=reinterpret_cast<Token*>(get<Word>(self,0x40)+at); // 0041E000..0041E003
                                                MethodsOf(self)->token_value(self,nullptr,token,&value); // 0041DFF5..0041E006 call [edx]
                                            }
                                        }
                                        break;
                                    }
                                }while(high>low);
                            }
                        }
                    }
                    append(result,&value);                     // 0041E008..0041E036
                    start=i+1;                                 // 0041E016 lea esi,[ebp+1]
                }
            }
            // 0041E03F: mov al,[ebx+ebp+1]; inc ebp; test al,al. After an unterminated token
            // i is at the NUL and this reads the byte after it (original over-read, kept).
        }while(s[++i]);
    }
    if(start>0){                                               // 0041E04C test esi,esi; jle
        if(start<i&&get<Word>(self,0x50)==0){                  // 0041E050..0041E059
            String tail(s+start);                              // 0041E05B..0041E062 005E5A90 (state 7)
            append(result,&tail);                              // 0041E067..0041E093
        }                                                      // 0041E0A1
        input->Assign005e5c50(&result);                        // 0041E0A6..0041E0AF
    }
}                                                              // 0041E0B4..0041E0CE value, result destroyed
}
