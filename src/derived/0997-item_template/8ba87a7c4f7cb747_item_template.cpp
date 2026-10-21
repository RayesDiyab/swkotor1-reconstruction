#include "b582d57c06a208e2_item_template.hpp"
namespace recovered_item_template{U load(U self,U ref,U scratch,Ports&p){U allocation=p.allocate(0xa0);U gff=allocation?p.construct(allocation,0x7e9,"UTI ",ref):0;if(!p.read(gff+0x94)){p.destroy(gff);return 0;}p.root(gff,scratch);p.fields(self,gff,scratch);p.destroy(gff);return 1;}}
