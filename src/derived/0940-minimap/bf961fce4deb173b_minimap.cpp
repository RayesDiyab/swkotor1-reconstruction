#include "a9beba76e8c9e9c7_minimap.hpp"
#include <cstring>
namespace recovered_minimap {
static I signed_word(U v){I out;std::memcpy(&out,&v,4);return out;}
void setup(U self,const U*a,Ports&p){
 if(!a[0])return;
 I width=signed_word(a[1]);if(width>88||width==0)width=88;
 p.write(self,8,static_cast<U>(width));I height=p.height(width);p.write(self,12,static_cast<U>(height));if(height<1)height=1;
 p.write(self,16,a[2]);const U owned=p.read(self,0);p.write(self,12,static_cast<U>(height));p.write(self,20,p.reciprocal(signed_word(a[11])));if(owned)return;
 const U product=(static_cast<U>(height)+1u)*(static_cast<U>(width)+1u);const I count=signed_word(product)/32;const U words=static_cast<U>(count)+1u;
 p.write(self,4,words);const U storage=p.allocate(words*4u);const U bytes=p.read(self,4)*4u;p.write(self,0,storage);p.clear(storage,bytes);
 U mapping_args[12];std::memcpy(mapping_args,a,sizeof(mapping_args));mapping_args[2]=p.read(self,16);
 p.mapping(self,mapping_args);
}
}
