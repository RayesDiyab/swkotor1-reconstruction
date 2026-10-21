#include "31535007a6dac6ab_event_action.hpp"
namespace kotor::scene_event {
namespace {
void destroy_strings(Memory16& m,Services& s,Word data){
    const Word count=m.read(data-4u);
    s.destruct_string_array_006fa852(data,8,count,0x005e5c20u);
    s.release(data-4u);
}
void clear_scalar_list(Memory16& m,Services& s,Word list){
    const Word data=m.read(list);
    if(data!=0){s.release(data);m.write(list,0);m.write(list+8u,0);}
    m.write(list+4u,0);
}
}
void destroy_event_004d7590(Memory16& m,Services& s,Word self){
    clear_scalar_list(m,s,self+4u);
    clear_scalar_list(m,s,self+0x10u);
    clear_scalar_list(m,s,self+0x28u);
    Word data=m.read(self+0x1cu);
    if(data!=0){destroy_strings(m,s,data);m.write(self+0x1cu,0);m.write(self+0x24u,0);}
    m.write(self+0x20u,0);
    // Preserve compiler-emitted member teardown, including repeated null frees
    // and live reloads. These are observable calls, not redundant cleanup.
    s.release(m.read(self+0x28u));m.write(self+0x28u,0);
    data=m.read(self+0x1cu);
    if(data!=0)destroy_strings(m,s,data);
    m.write(self+0x1cu,0);
    s.release(m.read(self+0x10u));m.write(self+0x10u,0);
    s.release(m.read(self+4u));m.write(self+4u,0);
}
}
