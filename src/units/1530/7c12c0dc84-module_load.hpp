#pragma once
#include "997d282d7e-module_start.hpp"
#include "6173e8e313-module_metadata.hpp"
namespace kotor::server_mainloop {
struct ModuleLoadPorts:ModuleStartPorts,ModuleMetadataPorts {
 virtual Word literal_005e5a90(Word,Word)override=0;
 virtual Word short_00411b40(Word,Word,Word,Word,Word)=0;
 virtual void date_004aebd0(Word,Word,Word,Word,Word,Word,Word,Word)=0;
 virtual Word tick_004af0d0(Word)=0;
 virtual Word day_004af0b0(Word)=0;
 virtual Word timer_004aede0(Word)=0;
 virtual Word timer_004aedf0(Word)=0;
 virtual Word timer_004aee00(Word)=0;
 virtual void timer_scale_004adba0(Word,Word)=0;
 virtual void timer_date_004ae3e0(Word,Word,Word,Word,Word,Word,Word,Word,Word)=0;
 virtual void timer_copy_004adef0(Word,Word)=0;
 virtual void timer_fields_004ade40(Word,Word,Word)=0;
 virtual Word list_004118c0(Word,Word,Word,Word)=0;
 virtual Word count_00411940(Word,Word)=0;
 virtual void element_00411990(Word,Word,Word,Word)=0;
 virtual Word allocate(Word)=0;
 virtual void ref_empty_00405ed0(Word)=0;
 virtual void append_005e9dc0(Word,Word)=0;
 virtual void resource_release_00409b80(Word)=0;
 virtual void ref_string_00405f70(Word,Word)=0;
 virtual Wide wide_00411d70(Word,Word,Word,Word,Wide)=0;
 virtual void talk_0041db50(Word,Word,Word)=0;
 virtual void variables_0059aa80(Word,Word,Word)=0;
 virtual void effects_0059b0f0(Word,Word,Word)=0;
 virtual Word messages_004aed80(Word)=0;
 virtual void messages_load_004b0a00(Word,Word,Word)=0;
 virtual Word party_004aee70(Word)=0;
 virtual void party_begin_005638d0(Word)=0;
 virtual void party_end_00563950(Word)=0;
 virtual Word resource_exists_00408c00(Word,Word,Word,Word)=0;
 virtual Word gff_construct_00410630(Word,Word,Word,Word)=0;
 virtual void resource_destroy(Word,Word)=0;
 virtual Word faction_load_0052b5c0(Word,Word,Word)=0;
 virtual void reputation_load_0052bbe0(Word,Word,Word)=0;
 virtual Word faction_check_0052b490(Word,Word)=0;
 virtual void faction_a_0052b9e0(Word)=0;
 virtual void faction_b_0052bce0(Word)=0;
 virtual void finish_004c8c70(Word,Word,Word,Word)=0;
 virtual Word message_service_004aedb0(Word)=0;
 virtual void message_date_0056c910(Word,Word,Word)=0;
};
void module_time_004c9597(Memory&,ModuleLoadPorts&,Word,Word,Word);
Word module_load_004c9050(Memory&,ModuleLoadPorts&,Word,Word,Word,Word);
}
