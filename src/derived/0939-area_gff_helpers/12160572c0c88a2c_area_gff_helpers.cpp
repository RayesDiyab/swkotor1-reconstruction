#include "4aaf0c8f40455153_area_gff_helpers.hpp"
namespace kotor::server_mainloop {
Word gff_field_type_00411880(Memory&m,GffExtraPorts&p,Word self,Word root,Word label,Word index){
 if(label)index=p.label_00411630(self,root,label);
 Word field=p.field_00410990(self,root,index);return field?m.read(field):0xffffffff;
}
}
