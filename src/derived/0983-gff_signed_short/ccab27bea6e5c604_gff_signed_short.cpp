#include "b142c7cb5c04110b_gff_signed_short.hpp"
namespace kotor::server_mainloop {
Word gff_signed_short_00411bb0(Memory&m,GffExtraPorts&p,Word self,Word record,Word label,Word success,Word fallback){if(record&&label){Word index=p.label_00411630(self,record,label);Word field=p.field_00410990(self,record,index);if(field&&m.read(field)==3){m.write(success,1);return m.read16(field+8);}}m.write(success,0);return fallback&65535;}
}
