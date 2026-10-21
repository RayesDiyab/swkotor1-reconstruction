#include "b270893e33f13707_mesh_segment_clip.hpp"
namespace kotor::server_mainloop {
Word mesh_segment_clip_00583e30(Memory&m,MeshSegmentClipPorts&p,Word mesh,Word from,Word to,Word outFrom,Word outTo,Word edge,Word s){
 auto copy=[&](Word dst,Word src){Word x=m.read(src),y=m.read(src+4);m.write(dst,x);Word z=m.read(src+8);m.write(dst+4,y);m.write(dst+8,z);};
 auto copyPoint=[&](Word dst,Word src){m.write(dst,m.read(src));m.write(dst+4,m.read(src+4));m.write(dst+8,m.read(src+8));};
 copy(s+0x50,from);copy(s+0x5c,to);
 m.write(s+0x10,m.read(s+0x50));m.write(s+0x14,m.read(s+0x54));m.write(s+0x18,0);m.write(s+0x1c,m.read(s+0x5c));m.write(s+0x20,m.read(s+0x60));m.write(s+0x24,0);
 for(Word o=0x34;o<0x4c;o+=4)m.write(s+o,0);
 if(p.equal(s+0x10,s+0x1c)){m.write(s+0x20,p.plus_one(m.read(s+0x20)));Word count=p.crossings(mesh,s+0x10,s+0x1c,edge);copy(outFrom,s+0x50);copy(outTo,s+0x5c);return count&1;}
 Word count=p.crossings(mesh,s+0x10,s+0x1c,edge);p.length(s);
 if(!count){copy(outFrom,s+0x50);copy(outTo,s+0x5c);return 0;}
 Word point=m.read(mesh+0xbc);
 if(count&1){if(!p.first_compare(s,point,false)){copy(outFrom,s+0x50);copy(outTo,s+0x5c);}else{copy(outFrom,s+0x50);copyPoint(outTo,m.read(mesh+0xbc));}return 1;}
 if(!p.first_compare(s,point,true)){copy(outFrom,s+0x50);copy(outTo,s+0x5c);return 0;}
 if(!p.second_compare(s)){copyPoint(outFrom,point);copy(outTo,s+0x5c);return 1;}
 copyPoint(outFrom,point);copyPoint(outTo,m.read(mesh+0xbc)+12);return 1;
}
}
