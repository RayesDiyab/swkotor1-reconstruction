#include "d593e8985d2100b3_gff_write_localized_getters.hpp"
namespace kotor::server_mainloop {
static Word locate(Memory&m,LocalizedGetterPorts&p,Word internal,Word index,Word scratch){if(index>=m.read(internal+4))return 0;Word node=m.read(m.read(internal));m.write(scratch,node);Word value=node?m.read(node+8):0;Word i=0;while(value){if(i==index)return value;value=p.next(m.read(internal),scratch);++i;}return 0;}
Word localized_get_index_005e9eb0(Memory&m,LocalizedGetterPorts&p,Word self,Word index,Word language,Word text,Word gender,Word scratch){Word value=locate(m,p,m.read(self),index,scratch),result=0;if(value){m.write(language,m.read(value));p.assign(text,value+4);result=1;}
 // Original wrapper transforms the language output even when lookup failed.
 auto signedValue=static_cast<std::int32_t>(m.read(language));p.byte(gender,static_cast<Word>(signedValue%2)&255);m.write(language,static_cast<Word>(signedValue/2));return result;}
Word localized_length_index_005e9f00(Memory&m,LocalizedGetterPorts&p,Word self,Word index,Word scratch){Word value=locate(m,p,m.read(self),index,scratch);return value?p.length(value+4):0;}
}
