#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
// Restores only004C919B..004C9597 of module startup. Caller continues afterward.
struct ModuleMetadataPorts {
 virtual ~ModuleMetadataPorts()=default;
 virtual void root_00411240(Word,Word)=0;
 virtual Word byte_00411a60(Word,Word,Word,Word,Word)=0;
 virtual Word string_00411ec0(Word,Word,Word,Word,Word,Word)=0;
 virtual void assign_string_005e5c50(Word,Word)=0;
 virtual void destroy_string_005e5c20(Word)=0;
 virtual Word resource_path_00407780(Word,Word,Word)=0;
 virtual void data_00412380(Word,Word,Word,Word,Word,Word,Word)=0;
 virtual Word signed_00411c90(Word,Word,Word,Word,Word)=0;
 virtual Word word_00411c20(Word,Word,Word,Word,Word)=0;
 virtual Word module_id_004aec90(Word)=0;
 virtual void update_id_0054eaa0(Word,Word)=0;
 virtual Word loc_empty_005e9e10(Word)=0;
 virtual Word loc_read_00411fd0(Word,Word,Word,Word,Word,Word)=0;
 virtual void loc_assign_005e9e70(Word,Word)=0;
 virtual void loc_destroy_005ea000(Word)=0;
 virtual void ref_literal_00406d80(Word,Word)=0;
 virtual Word ref_read_00411e10(Word,Word,Word,Word,Word,Word)=0;
 virtual void ref_assign_00406120(Word,Word,Word)=0;
 virtual Word literal_005e5a90(Word,Word)=0;
 virtual void set_tag_004c3060(Word,Word)=0;
 // Float reader returns the stored32-bit result of original x87 return.
 virtual Word float_bits_00411d00(Word,Word,Word,Word,Word)=0;
 virtual void write_byte(Word,Word)=0;
};
void module_identity_004c919b(Memory&,ModuleMetadataPorts&,Word,Word,Word);
}
