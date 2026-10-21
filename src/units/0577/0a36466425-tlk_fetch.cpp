// lane-tlk: CTlkTable::FetchInternal 0041E1A0 and CTlkTable::Fetch 0041E550.
// Evidence: dumpbin /disasm swkotor.exe 0x41E1A0-0x41E550, 0x41E550-0x41E597.
#include "6db30bdc1c-tlk_core.h"
#include "c7e191db10-native_strings.h"     // WindowCStr005e5670 (005E5670)
#include "689940810d-string_priority.h"    // GuiAssign005e5140 (005E5140)
#include "9281d0a58c-warning_runtime.h"    // StartupFormat005e5680 (005E5680), warning_empty_0073d71c
#include "7b8c81fcff-allocator_live.h"     // live_operator_new (006FA7E6), live_free (006FA390)
#include "fba81ecfbc-size_seek.h"          // exofile_size_005e6950, exofile_seek_005e6aa0

namespace {
using namespace tlk_core;
// Original read-only literals (no identity comparison is made on them).
const char invalid_strref_0073ed04[]="Invalid STRREF %d passed to Fetch"; // 0041E1EA push 73ED04h
const char simple_format_0073ed28[]="[%d]%s";                              // 0041E580 push 73ED28h
const char* empty_0073d71c(){return base_ini_reconstruction::warning_empty_0073d71c;} // 0073D71C ""

// One TLK string-table record as read at 0041E29F into [esp+48h] (36 or 40 bytes).
struct Record {
    Word flags;          // +00 [esp+48h]: bit0 text, bit1 sound, bit2 duration, bit15 reject
    Ref sound;           // +04 [esp+4Ch]
    Word volume_variance;// +14
    Word pitch_variance; // +18
    Word text_offset;    // +1C [esp+64h]
    Word text_length;    // +20 [esp+68h]
    Word duration_bits;  // +24 [esp+6Ch] (only read for 40-byte records; zeroed before each read)
};
static_assert(sizeof(Record)==40 && offsetof(Record,sound)==4 && offsetof(Record,text_offset)==0x1c &&
              offsetof(Record,text_length)==0x20 && offsetof(Record,duration_bits)==0x24,"observed record");

TlkFile* slot_file(Tlk* self,unsigned index){return get<TlkFile*>(self,4u+4u*index);} // [ebx+edx*4+4]
Word version_word(){                                           // 0041E233..0041E24D byte-wise dword
    const unsigned char* v=reinterpret_cast<const unsigned char*>(kotor_cell_tlk_version_0078d3ec);
    return static_cast<Word>(v[0])|(static_cast<Word>(v[1])<<8)|(static_cast<Word>(v[2])<<16)|(static_cast<Word>(v[3])<<24);
}
// Clear tail shared by 0041E4E5 / 0041E4A4 / 0041E47D: text = "" (005E5140), sound = "" (00406290).
void clear_text(StringResource* output){(void)GuiAssign005e5140(&output->text,nullptr,empty_0073d71c());}
void clear_sound(StringResource* output){
    Ref returned;                                              // by-value CResRef return slot (trivial)
    (void)unresolved_ref_assign_00406290(&output->sound,nullptr,&returned,empty_0073d71c());
}
// Raw storage for a CExoString built by a constructor-style provider (005E5B70/005E5BC0)
// and destroyed by 005E5C20 at the observed point.
struct BuiltString {
    alignas(String) unsigned char storage[sizeof(String)];
    String* raw(){return reinterpret_cast<String*>(storage);}
};
}

namespace tlk_core {
int __fastcall FetchInternal0041e1a0(Tlk* self,void*,Word strref,StringResource* output,int parse) {
    String text;                                               // [esp+18h] 0041E1C6 (EH state 0)
    String diagnostic;                                         // [esp+20h] 0041E1D9 (EH state 1)
    kotor_reconstruction::StartupFormat005e5680(&diagnostic,invalid_strref_0073ed04,strref); // 0041E1E5..0041E1F8 (result unused)
    if(strref==0xffffffffu||get<unsigned char>(self,0x3c)>1u){ // 0041E200 je / 0041E209 cmp byte [ebx+3Ch],1; ja
        clear_text(output);                                    // 0041E4EC..0041E4F3
        clear_sound(output);                                   // 0041E4F8..0041E505
        return 1;                                              // 0041E529 (locals destroyed 0041E50A..0041E524)
    }
    Word entry=strref;                                         // ebp
    Word found=0;                                              // [esp+14h]
    TlkFile* selected=nullptr;                                 // esi
    Record record{};                                           // [esp+48h]; zero-init is unobservable (C4701):
                                                               // record is read only after found==1, i.e. after a full 005E6960 fill
    for(Word bank=0;static_cast<std::int32_t>(bank)<14;bank+=2){ // [esp+80h] 0041E2B9..0041E2CD (add 2; cmp 0Eh; jl)
        TlkFile* file=slot_file(self,get<unsigned char>(self,0x3c)+bank); // 0041E221..0041E227
        if(!file)continue;                                     // 0041E22B
        const Word stride=(file->header[1]==version_word())?40u:36u; // 0041E237 [esi+8]; sete; lea [edx*4+24h]
        entry&=0x00ffffffu;                                    // 0041E259
        if(entry>=file->header[3])continue;                    // 0041E253 [esi+10h]; 0041E25F cmp; jae
        const Word offset=stride*entry+0x14u;                  // 0041E26C..0041E273
        if(offset<=file_internal::exofile_size_005e6950(&file->file,nullptr) &&            // 0041E276..0041E27D ja
           file_internal::exofile_seek_005e6aa0(&file->file,nullptr,offset,0)) {           // 0041E27F..0041E28B
            record.duration_bits=0;                            // 0041E297 mov [esp+78h],0 (= [esp+6Ch] after 3 pushes)
            if(file_internal::exofile_read_005e6960(&file->file,nullptr,&record,1,stride)==stride && // 0041E29F..0041E2A6
               !(record.flags&0x8000u)) {                      // 0041E2A8..0041E2AE test ah,ah; jns
                found=1;selected=file;break;                   // 0041E32D..0041E336
            }
        }
        // 0041E2B0 reload of [0078D3ECh] and this: re-read at the next version_word() call.
    }
    if(found){
        output->duration_bits=(record.flags&4u)?record.duration_bits:0u; // 0041E32D..0041E350
        Ref returned;                                          // [esp+28h] by-value return slot
        if(record.flags&2u)(void)SoundAssign00406170(&output->sound,nullptr,&returned,&record.sound); // 0041E35E..0041E36A
        else (void)unresolved_ref_assign_00406290(&output->sound,nullptr,&returned,empty_0073d71c()); // 0041E371..0041E37D
        if(record.flags&1u){                                   // 0041E382 test byte [esp+48h],1
            const Word start=selected->header[4];              // 0041E38D [esi+14h] saved in [esp+80h]
            if(start+record.text_offset>file_internal::exofile_size_005e6950(&selected->file,nullptr)){ // 0041E399..0041E3AD ja
                BuiltString numeric;                           // [esp+28h]
                String* value=resman_string_from_int_005e5bc0(numeric.raw(),nullptr,static_cast<int>(entry)); // 0041E450..0041E455
                output->text.Assign005e5c50(value);            // 0041E45B..0041E462 (EH state 2)
                value->~StartupString();                       // 0041E470
                clear_sound(output);                           // 0041E475..0041E4BD
                return 0;                                      // 0041E4E1 (B then A destroyed 0041E4C2..0041E4DC)
            }
            if(!file_internal::exofile_seek_005e6aa0(&selected->file,nullptr,selected->header[4]+record.text_offset,0)){ // 0041E3B3..0041E3C4 (reloaded [esi+14h])
                clear_text(output);                            // 0041E47D..0041E484
                clear_sound(output);                           // 0041E489..0041E4BD
                return 0;
            }
            void* buffer=live_operator_new(record.text_length); // 0041E3CA..0041E3CF
            (void)file_internal::exofile_read_005e6960(&selected->file,nullptr,buffer,1,record.text_length); // 0041E3D4..0041E3E3 (count ignored)
            {
                BuiltString counted;                           // [esp+28h]
                String* value=resman_string_counted_005e5b70(counted.raw(),nullptr,static_cast<const char*>(buffer),
                                                             static_cast<int>(record.text_length)); // 0041E3E8..0041E3F2
                text.Assign005e5c50(value);                    // 0041E3F7..0041E401 (EH state 3)
                value->~StartupString();                       // 0041E40F
            }
            live_free(buffer);                                 // 0041E414..0041E415
            if(parse)ParseStr0041dd30(self,nullptr,&text);     // 0041E41A..0041E431
            output->text.Assign005e5c50(&text);                // 0041E436..0041E442
        }
    }
    // 0041E2D3 / 0041E2DA common tail.
    if(get<unsigned char>(self,0x3c)==1u){                     // 0041E2DA cmp byte [ebx+3Ch],1; jne
        if(found)return 1;                                     // 0041E2E8 -> 0041E50A
        put<unsigned char>(self,0x3c,0);                       // 0041E2FC
        return FetchInternal0041e1a0(self,nullptr,entry,output,parse); // 0041E2F0..0041E300 (locals destroyed after)
    }
    if(found)return 1;                                         // 0041E4A0..0041E4A2
    clear_text(output);                                        // 0041E4A4..0041E4AB
    clear_sound(output);                                       // 0041E4B0..0041E4BD ([esp+38h] return slot)
    return 0;                                                  // 0041E4E1
}
}

// 0041E550 CTlkTable::Fetch: [esi+3Ch]=gender byte (0041E550/0041E566), FetchInternal(strref,out,0),
// then if dword [esi+54h]: out->text = format("[%d]%s", strref, out->text.CStr()) (0041E570..0041E586).
extern "C" tlk_core::Word __fastcall tlk_fetch_0041e550(tlk_core::Tlk* self,void*,tlk_core::Word strref,
                                                        tlk_core::StringResource* output,tlk_core::Word gender) {
    using namespace tlk_core;
    put<unsigned char>(self,0x3c,static_cast<unsigned char>(gender)); // 0041E550 mov al,[esp+0Ch]; 0041E566
    const int result=FetchInternal0041e1a0(self,nullptr,strref,output,0); // 0041E560..0041E569
    if(get<Word>(self,0x54)!=0){                               // 0041E570..0041E575
        const char* current=WindowCStr005e5670(&output->text,nullptr); // 0041E577..0041E579
        kotor_reconstruction::StartupFormat005e5680(&output->text,simple_format_0073ed28,strref,current); // 0041E57E..0041E586
    }
    return static_cast<Word>(result);                          // 0041E591 mov eax,ebx; ret 0Ch
}
