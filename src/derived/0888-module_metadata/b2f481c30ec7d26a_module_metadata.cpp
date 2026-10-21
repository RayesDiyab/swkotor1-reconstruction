#include "8bea7612e96e69a5_module_metadata.hpp"
namespace kotor::server_mainloop {
void module_identity_004c919b(Memory& m,ModuleMetadataPorts& p,Word self,Word name,Word scratch){
 auto s=[scratch](Word n){return scratch+n;};auto resource=[&](){return m.read(self+8);};
 p.root_00411240(resource(),s(0x10));
 m.write(self+0x1c8,p.byte_00411a60(resource(),s(0x10),0x745cc0,s(0xc),m.read8(self+0x1c8))&255);
 m.write(self+0x200,p.byte_00411a60(resource(),s(0x10),0x745cb0,s(0xc),m.read8(self+0x200))&255);
 if(m.read(self+0x200)==1){Word value=p.string_00411ec0(resource(),s(0x20),s(0x10),0x745ca0,s(0xc),self+0x204);p.assign_string_005e5c50(self+0x204,value);p.destroy_string_005e5c20(s(0x20));}
 m.write(self+0x20c,p.resource_path_00407780(m.read(0x7a39e8),resource(),m.read(self+0x200)==0));
 p.data_00412380(resource(),s(0x10),m.read(self+0x54),32,0x745d08,s(0xc),0);
 Word value=p.signed_00411c90(resource(),s(0x10),0x745cf8,s(0xc),0);m.write(m.read(self+0x54)+0x20,value);
 value=p.word_00411c20(resource(),s(0x10),0x745cec,s(0xc),0);m.write(m.read(self+0x54)+0x24,value);
 p.update_id_0054eaa0(p.module_id_004aec90(m.read(m.read(0x7a39fc)+8)),name);
 for(unsigned i=0;i<2;i++){
  Word fallback=p.loc_empty_005e9e10(s(0x38));
  Word text=p.loc_read_00411fd0(resource(),s(0x20),s(0x10),i?0x745cd0:0x745ce0,s(0xc),fallback);
  p.loc_assign_005e9e70(self+(i?0x4c:0x64),text);
  p.loc_destroy_005ea000(s(0x20));p.loc_destroy_005ea000(s(0x38));
 }
 p.ref_literal_00406d80(s(0x28),0x73d71c);
 value=p.ref_read_00411e10(resource(),s(0x48),s(0x10),0x745da4,s(0xc),s(0x28));
 p.ref_assign_00406120(self+0x7c,s(0x9c),value);
 p.literal_005e5a90(s(0x20),0x73d71c);
 value=p.string_00411ec0(resource(),s(0x38),s(0x10),0x745c3c,s(0xc),s(0x20));
 p.set_tag_004c3060(self,value);p.destroy_string_005e5c20(s(0x38));p.destroy_string_005e5c20(s(0x20));
 p.ref_literal_00406d80(s(0x28),0x73d71c);
 value=p.ref_read_00411e10(resource(),s(0x48),s(0x10),0x745c2c,s(0xc),s(0x28));
 p.ref_assign_00406120(m.read(self+0x58),s(0x9c),value);
 const Word labels[]={0x745c20,0x745c14,0x745c08,0x745bf8,0x745be8};
 for(Word i=0;i<5;i++){value=p.float_bits_00411d00(resource(),s(0x10),labels[i],s(0xc),0);m.write(m.read(self+0x58)+0x10+4*i,value);}
 if(!m.read(s(0xc))){m.write(m.read(self+0x58)+0x1c,0x3f800000);m.write(m.read(self+0x58)+0x20,0);}
 const Word byte_labels[]={0x745bd8,0x745bc8,0x745bb8};
 for(Word i=0;i<3;i++)p.write_byte(self+0x1a0+i,p.byte_00411a60(resource(),s(0x10),byte_labels[i],s(0xc),0));
}
}
