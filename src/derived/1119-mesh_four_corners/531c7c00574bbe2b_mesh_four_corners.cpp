#include "b5d46174bdb62a66_mesh_four_corners.hpp"
namespace kotor::server_mainloop {
static void copy_corner(Memory&m,Word out,Word source){for(Word i=0;i<12;i+=4)m.write(out+i,m.read(source+i));}
Word mesh_four_corners_00596e40(Memory&m,MeshBoundsPorts&p,Word self,Word a,Word b,Word c,Word d,Word s){
 Word vertex=m.read(self+0x54);m.write(s+0x14,self);if(!vertex)return 0;
 Word count=m.read(self+0x50);m.write(s+0x10,0);for(Word i=0;i<24;i+=4)m.write(s+0x18+i,0);
 if(static_cast<int>(count)>0){do{
  Word x=m.read(vertex);m.write(s+0x30,x);Word y=m.read(vertex+4),z=m.read(vertex+8);m.write(s+0x38,z);Word initialized=m.read(s+0x10);m.write(s+0x34,y);
  if(!initialized){Word xx=m.read(vertex);m.write(s+0x24,xx);Word yy=m.read(vertex+4),zz=m.read(vertex+8);m.write(s+0x2c,zz);m.write(s+0x28,yy);xx=m.read(vertex);m.write(s+0x18,xx);yy=m.read(vertex+4);zz=m.read(vertex+8);m.write(s+0x1c,yy);m.write(s+0x10,1);m.write(s+0x20,zz);}
  else{for(Word i=0;i<12;i+=4){if(p.greater(m.read(s+0x30+i),m.read(s+0x24+i)))m.write(s+0x24+i,m.read(s+0x30+i));if(p.less(m.read(s+0x30+i),m.read(s+0x18+i)))m.write(s+0x18+i,m.read(s+0x30+i));}}
  vertex+=12;--count;
 }while(count);}
 Word x=m.read(s+0x18),y=m.read(s+0x1c),z=m.read(s+0x20);
 m.write(a,x);m.write(a+4,y);m.write(a+8,z);m.write(a+4,m.read(s+0x28));m.write(a+8,z);
 m.write(b,x);m.write(b+4,y);Word maxX=m.read(s+0x24);m.write(b+8,z);Word maxY=m.read(s+0x28);m.write(b+8,z);
 m.write(c,maxX);m.write(c+4,maxY);m.write(c+8,m.read(s+0x2c));m.write(c+4,m.read(s+0x1c));m.write(c+8,m.read(s+0x20));
 m.write(d,maxX);m.write(d+4,m.read(s+0x28));m.write(d+8,m.read(s+0x2c));m.write(d+8,m.read(s+0x20));
 Word out=p.transform(self,s+0x30,a);copy_corner(m,a,out);Word liveSelf=m.read(s+0x14);
 out=p.transform(liveSelf,s+0x24,b);copy_corner(m,b,out);
 out=p.transform(liveSelf,s+0x18,c);copy_corner(m,c,out);
 out=p.transform(liveSelf,s+0x3c,d);Word ox=m.read(out);m.write(d,ox);Word oy=m.read(out+4);m.write(d+4,oy);Word oz=m.read(out+8);Word result=m.read(s+0x10);m.write(d+8,oz);return result;
}
}
