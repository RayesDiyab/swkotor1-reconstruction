#include "90b93f23fd3c0308_placeable_template.hpp"
namespace kotor::server_mainloop {
Word placeable_template_00587a70(Memory&m,PlaceableTemplatePorts&p,Word self,Word ref,Word flag,Word s){Word storage=p.allocate(0xa0);Word gff=storage?p.construct(storage,0x7fc,0x749850,ref):0;if(!m.read(gff+0x94)){p.destroy(gff);p.string_zero(s+0x18);p.string_zero(s+0x10);p.ref_string(ref,s+0x10);Word text=p.text(s+0x10);p.format(s+0x18,0x74982c,text);p.string_destroy(s+0x10);p.string_destroy(s+0x18);return 0;}p.root(gff,s+0xc);p.fields(self,gff,s+0xc,flag);p.destroy(gff);return 1;}
}
