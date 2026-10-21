#include "0b83e5656d63f2d8_placeable_light.hpp"
namespace recovered_placeable_light{
U get(U s,U scratch,Ports&p){p.string(scratch+4);U row=p.byte(s+0x230);U table=p.table();U ok=p.query(table,row,scratch+4,scratch);p.destroy(scratch+4);return ok?p.read(s+0x38c):0;}
void set(U s,U value,U scratch,Ports&p){if(p.read(s+0x38c)==value)return;p.string(scratch+4);U row=p.byte(s+0x230);U table=p.table();U ok=p.query(table,row,scratch+4,scratch);p.destroy(scratch+4);if(ok){p.write(s+0x390,1);p.write(s+0x38c,value);}}
}
