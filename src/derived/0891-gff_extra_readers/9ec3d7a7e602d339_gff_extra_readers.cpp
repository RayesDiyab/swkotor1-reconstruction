#include "d697cdc7d3c3da57_gff_extra_readers.hpp"
namespace kotor::server_mainloop {
Word gff_short_00411b40(Memory&m,GffExtraPorts&p,Word self,Word record,Word label,Word success,Word fallback){
 if(record&&label){Word index=p.label_00411630(self,record,label);Word field=p.field_00410990(self,record,index);if(field&&m.read(field)==2){m.write(success,1);return m.read16(field+8);}}
 m.write(success,0);return fallback&65535;
}
Wide gff_wide_00411d70(Memory&m,GffExtraPorts&p,Word self,Word record,Word label,Word success,Wide fallback,Word scratch){
 if(record&&label){Word index=p.label_00411630(self,record,label);Word field=p.field_00410990(self,record,index);if(field&&m.read(field)==6){Word data=p.data_00410a20(self,field,scratch);if(data&&m.read(scratch)>=8){m.write(success,1);Word low=m.read(data),high=m.read(data+4);return Wide(low)|(Wide(high)<<32);}}}
 m.write(success,0);return fallback;
}
Word gff_data_00412380(Memory&m,GffExtraPorts&p,Word self,Word record,Word dest,Word capacity,Word label,Word success,Word fallback,Word scratch){
 if(record&&label&&dest){Word index=p.label_00411630(self,record,label);Word field=p.field_00410990(self,record,index);if(field&&m.read(field)==13){Word data=p.data_00410a20(self,field,scratch);if(data&&m.read(scratch)>=4){Word length=m.read(data);if(m.read(scratch)>=length+4){m.write(success,1);Word n=length<capacity?length:capacity;for(Word i=0;i<(n>>2);i++)m.write(dest+i*4,m.read(data+4+i*4));for(Word i=n&~Word(3);i<n;i++)p.write_byte(dest+i,m.read8(data+4+i));return dest;}}}}
 m.write(success,0);return fallback;
}
}
