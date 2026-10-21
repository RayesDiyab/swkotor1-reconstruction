#include "534aed964914cc17_vm_string_live.hpp"
namespace recovered_vm_string {
U push(Ports&p,U vm,U source){U allocation=p.allocate(8),result=0;if(allocation){U chars=p.chars(source);result=p.construct(allocation,chars);}U s=vm+0x1ac;recovered_vm_scalar::add(p,s,5);U depth=p.read(s),words=p.read(vm+0x1bc);p.write(words+depth*4-4,result);return 1;}
U pop(Ports&p,U vm,U destination){U depth=p.read(vm+0x1ac);if(!depth||(depth&0x80000000u))return 0;U tags=p.read(vm+0x1b8),tag=p.byte(tags+depth-1);U top=depth-1;if(tag!=5)return 0;U words=p.read(vm+0x1bc),source=p.read(words+top*4);p.assign(destination,source);p.shrink(vm+0x1ac,top);return 1;}
}
