#include "24711d6d778a359c_vm_stack_view.hpp"
#include "ef338a99d727da76_stack_clear.h"
#include "db58c87c3deeee77_script_delete.h"
#include <cstring>
namespace recovered_vm_stack_view{
namespace{
struct Context{U stack;Ports&p;};Context& c(void*v){return *static_cast<Context*>(v);}void* pointer(U v){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(v));}U word(void*v){return static_cast<U>(reinterpret_cast<std::uintptr_t>(v));}I signed_bits(U u){I i;std::memcpy(&i,&u,4);return i;}
I depth(void*v){auto&a=c(v);return signed_bits(a.p.read(a.stack));}void set_depth(void*v,I n){auto&a=c(v);a.p.write(a.stack,static_cast<U>(n));}
std::uint8_t tag(void*v,I i){auto&a=c(v);return a.p.byte(a.p.read(a.stack+0xc)+static_cast<U>(i));}
U value(void*v,I i){auto&a=c(v);return a.p.read(a.p.read(a.stack+0x10)+static_cast<U>(i)*4);}
void* implementer(void*v){auto&a=c(v);return pointer(a.p.read(a.p.read(a.stack+0x14)+0x3d4));}
void* tags(void*v){auto&a=c(v);return pointer(a.p.read(a.stack+0xc));}void* words(void*v){auto&a=c(v);return pointer(a.p.read(a.stack+0x10));}
void clear_word(void*v,I i){auto&a=c(v);a.p.write(a.p.read(a.stack+0x10)+static_cast<U>(i)*4,0);}
void set_base(void*v,I n){auto&a=c(v);a.p.write(a.stack+4,static_cast<U>(n));}void set_capacity(void*v,U n){auto&a=c(v);a.p.write(a.stack+8,n);}void set_tags(void*v,void*p){auto&a=c(v);a.p.write(a.stack+0xc,word(p));}void set_words(void*v,void*p){auto&a=c(v);a.p.write(a.stack+0x10,word(p));}
void destroy_string(void*v,U h){c(v).p.destroy_string(h);}void release(void*v,U h){c(v).p.release(h);}void engine(void*v,void*obj,I type,U h){c(v).p.destroy_engine(word(obj),type,h);}void release_array(void*v,void*p){c(v).p.release(word(p));}
kotor::vm_stack_cleanup::View view(Context&a){return {&a,depth,set_depth,tag,value,implementer};}
kotor::vm_stack_cleanup::Dependencies dependencies(Context&a){return {&a,destroy_string,release,engine};}
kotor::vm_stack_clear::View clear_view(Context&a){return {view(a),tags,words,clear_word,set_base,set_capacity,set_tags,set_words};}
struct DescriptorContext{U descriptor;Ports&p;};
U descriptor_read(void*v,U off){auto&a=*static_cast<DescriptorContext*>(v);return a.p.read(a.descriptor+off);}
void descriptor_write(void*v,U off,U value){auto&a=*static_cast<DescriptorContext*>(v);a.p.write(a.descriptor+off,value);}
kotor::vm_stack_clear::View resolve_stack(void*v,U address){auto&a=c(v);a.stack=address;return clear_view(a);}
}
U delete_script(U descriptor,Ports&p){Context a{0,p};DescriptorContext d{descriptor,p};kotor::vm_delete::Dependencies deps{&a,resolve_stack,{dependencies(a),release_array},release};return kotor::vm_delete::delete_script_005d44f0({&d,descriptor_read,descriptor_write},deps);}
void clear(U stack,Ports&p){Context a{stack,p};kotor::vm_stack_clear::View v{view(a),tags,words,clear_word,set_base,set_capacity,set_tags,set_words};kotor::vm_stack_clear::Dependencies d{dependencies(a),release_array};kotor::vm_stack_clear::clear_stack_005d2140(v,d);}
void shrink(U stack,I requested,Ports&p){Context a{stack,p};kotor::vm_stack_cleanup::set_stack_pointer_005d21f0(view(a),requested,dependencies(a));}
}
