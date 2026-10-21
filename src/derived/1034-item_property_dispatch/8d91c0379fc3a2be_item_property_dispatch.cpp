#include "655106ce52d0c1e3_item_property_dispatch.hpp"
namespace recovered_item_property_dispatch{U apply(U self,U item,U property,U creature,U slot,U flag,Ports&p){U type=p.half(property);if(type>=60)return 0;U fn=p.read(p.read(self+4)+4*type);return fn?p.invoke(fn,self,item,property,creature,slot,flag):0;}}
