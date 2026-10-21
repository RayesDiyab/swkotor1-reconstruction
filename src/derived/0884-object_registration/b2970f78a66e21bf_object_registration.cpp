#include "a0a23538f3948246_object_registration.hpp"
namespace kotor::server_mainloop {
Word object_register_004d80e0(Memory& m,ModuleAllocation& p,Word self,Word out,Word object,Word reverse){
 if(!object)return 4;
 if(reverse){m.write(out,m.read(self+0xc));m.write(self+0xc,m.read(self+0xc)-1);}
 else{m.write(out,m.read(self+4));m.write(self+4,m.read(self+4)+1);}
 Word bucket=m.read(out)&0xfff;
 Word node=p.allocate(12);
 if(node){m.write(node,0);m.write(node+4,0);m.write(node+8,0);}
 // Original allocation failure dereferences null; preserve that behavior.
 Word id=m.read(out);m.write(node+4,object);m.write(node,id);m.write(node+8,0);
 Word slot=m.read(self)+bucket*4;Word first=m.read(slot);
 if(!first){m.write(slot,node);return 0;}
 Word target=m.read(node),current=first,previous=0;
 while(m.read(current)>target){previous=current;current=m.read(current+8);if(!current){m.write(previous+8,node);return 0;}}
 if(!previous){m.write(node+8,first);m.write(m.read(self)+bucket*4,node);return 0;}
 m.write(node+8,m.read(previous+8));m.write(previous+8,node);return 0;
}
}
