// lane-tlk: CClientExoAppInternal::AddDownloadedResources 005F4180 (registers LIVEn dialog
// TLK files through CTlkTable::AddFile on [007A3A08], plus LIVEn resource archives) and the
// StartServices router entry for call site 005F8D4A.
// Evidence: dumpbin /disasm swkotor.exe 0x5F4180-0x5F4455. Body order follows the R22 Task8
// donor (download_service.cpp 4a9d200c), re-authored without the modern CallerLease.
#include "6db30bdc1c-tlk_core.h"
#include "689940810d-string_priority.h"    // GuiConcat005e5d10 (005E5D10)
#include "9281d0a58c-warning_runtime.h"    // StartupFormat005e5680 (005E5680)
#include "7772bffc2f-alias_path.h"         // alias_path::AliasListView::GetAliasPath (005E6890)
#include "7d1d224ac9-async_load.h"         // async_ref_construct_string_00406d60 (00406D60)
#include "5d852003b2-manager_cell.h"       // kotor_resource_demand::resource_manager_007a39e8 ([007A39E8])
#include "8ae0e4efda-bridges.h"            // client_services_start::Call, kotor_menu_service_contract_violation

namespace {
using namespace tlk_core;
// Original read-only literals (pushed by address; no identity comparison is made).
const char live_name_0074e9ac[]="live%d";                      // 005F41DB
const char live_alias_007453c0[]="LIVE%d";                     // 005F41EB
const char live_file_0074e99c[]="LIVE%d:live%d";               // 005F424B / 005F4299 / 005F4333
const char live_rim_0074e984[]="LIVE%d:RIMSXBOX\\live%d";      // 005F42E6
const char live_suffix_0074e980[]="DX";                        // 005F4353
const char live_rim_dx_0074e964[]="LIVE%d:RIMSXBOX\\live%ddx"; // 005F43BC
const char live_textures_0074e948[]="LIVE%d:OVERRIDE\\textures";// 005F43DF
menu_resources::Manager* manager(){return kotor_resource_demand::resource_manager_007a39e8;} // mov ecx,[007A39E8h] (re-read per call)
// CResRef(CExoString const&) 00406D60 into a fresh local slot, then CExoResMan::Exists(ref,type,0) 00408BC0.
Word probe(Ref* slot,String* name,Word type){
    (void)async_ref_construct_string_00406d60(slot,nullptr,name);
    return manager()->exists_00408bc0(slot,type,nullptr);
}
struct Destroy{String* s;~Destroy(){s->~StartupString();}};
}

namespace tlk_core {
void __fastcall AddDownloadedResources005f4180(void*,void*) {
    String name;                                               // [esp+10h] 005F419F..005F41A3 (EH state 0)
    String path;                                               // [esp+8]   005F41A8..005F41B7 (EH state 1)
    if(kotor_rdata_live_count_0073ecd2<=1u)return;             // 005F41BC cmp byte [0073ECD2h],1; jbe
    int index=1;                                               // 005F41CB mov esi,1
    do{
        kotor_reconstruction::StartupFormat005e5680(&name,live_name_0074e9ac,index);   // 005F41D6..005F41E1
        kotor_reconstruction::StartupFormat005e5680(&path,live_alias_007453c0,index);  // 005F41E6..005F41F1
        void* base=kotor_menu_10_19::dependencies::base_007a39e0;                       // 005F41F6 [007A39E0h]
        auto* aliases=get<alias_path::AliasListView*>(base,0x0c);                      // 005F41FB [eax+0Ch]
        String* resolved=aliases->GetAliasPath(&path,0);                               // 005F4201..005F4208
        if(resolved->BorrowedLength005e5790()!=0){                                      // 005F420D..005F4216
            Ref refs[5];                                       // [esp+28h],[esp+38h],[esp+48h],[esp+58h],[esp+68h]
            if(probe(&refs[0],&name,0x7e2u)){                  // 005F421C..005F4243 (TLK)
                kotor_reconstruction::StartupFormat005e5680(&path,live_file_0074e99c,index,index); // 005F4245..005F4251
                (void)AddFile0041d920(static_cast<Tlk*>(kotor_cell_tlk_007a3a08),nullptr,&path,index); // 005F4259..005F4265
            }
            if(probe(&refs[1],&name,0x270fu)){                 // 005F426A..005F4291
                kotor_reconstruction::StartupFormat005e5680(&path,live_file_0074e99c,index,index); // 005F4293..005F429F
                (void)manager()->fixed_004087e0(&path);        // 005F42A4..005F42B2
            }
            if(probe(&refs[2],&name,0xbbau)){                  // 005F42B7..005F42DE
                kotor_reconstruction::StartupFormat005e5680(&path,live_rim_0074e984,index,index); // 005F42E0..005F42EC
                (void)manager()->image_004087c0(&path);        // 005F42F1..005F42FF
            }
            if(probe(&refs[3],&name,0x7dbu)){                  // 005F4304..005F432B
                kotor_reconstruction::StartupFormat005e5680(&path,live_file_0074e99c,index,index); // 005F432D..005F4339
                (void)manager()->encapsulated_004087a0(&path,2u); // 005F4341..005F434E
            }
            alignas(String) unsigned char output_storage[sizeof(String)]; // [esp+20h] hidden concat result
            String* output;
            {
                String suffix(live_suffix_0074e980);           // 005F4353..005F435C (EH state 2)
                output=GuiConcat005e5d10(&name,nullptr,reinterpret_cast<String*>(output_storage),&suffix); // 005F4361..005F4377
            }                                                  // 005F4388 suffix destroyed (EH state 4: output live)
            Destroy output_lifetime{output};                   // destroyed at 005F43FF after the archive call
            // The concatenation result is unused: the next probe re-reads name ([esp+10h], 005F438D).
            if(probe(&refs[4],&name,0xbbau)){                  // 005F438D..005F43B4
                kotor_reconstruction::StartupFormat005e5680(&path,live_rim_dx_0074e964,index,index); // 005F43B6..005F43C2
                (void)manager()->image_004087c0(&path);        // 005F43CA..005F43D5
            }
            kotor_reconstruction::StartupFormat005e5680(&path,live_textures_0074e948,index); // 005F43DA..005F43E5
            (void)manager()->encapsulated_004087a0(&path,1u);  // 005F43EA..005F43FA
        }
        ++index;                                               // 005F4417
    }while(index<static_cast<int>(kotor_rdata_live_count_0073ecd2)); // 005F4410 movzx; 005F4418 cmp; jl
}                                                              // 005F4420..005F4440 path then name destroyed
}

// Router entry for StartServices 005F8D4A: ECX = client internal (unused by the body),
// no stack words, thiscall. The original EAX is dead at the call site.
extern "C" client_services_start::Word __cdecl kotor_menu_service_005F4180(const client_services_start::Call* call) {
    if(!call||call->site!=0x005f8d4au||call->target!=0x005f4180u||
       call->abi!=client_services_start::Abi::Thiscall||call->count!=0u||!call->self)
        kotor_menu_service_contract_violation(call);
    tlk_core::AddDownloadedResources005f4180(reinterpret_cast<void*>(call->self),nullptr);
    return 0u;
}
