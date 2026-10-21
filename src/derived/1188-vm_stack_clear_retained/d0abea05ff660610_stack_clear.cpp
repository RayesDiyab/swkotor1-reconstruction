#include "ef338a99d727da76_stack_clear.h"
namespace kotor::vm_stack_clear {
namespace {
vm_leaf::Internal& state(void* p) {return *static_cast<vm_leaf::Internal*>(p);}
void* tags(void* p) {return state(p).tags;}
void* words(void* p) {return state(p).words;}
void clear_word(void* p,I32 i) {state(p).words[i]=0;}
void set_base(void* p,I32 n) {state(p).base_pointer=n;}
void set_capacity(void* p,U32 n) {state(p).capacity=n;}
void set_tags(void* p,void* q) {state(p).tags=static_cast<std::uint8_t*>(q);}
void set_words(void* p,void* q) {state(p).words=static_cast<U32*>(q);}
}
View view(vm_leaf::Internal& s) noexcept {
    return {vm_stack_cleanup::view(s),tags,words,clear_word,set_base,set_capacity,set_tags,set_words};
}
void clear_stack_005d2140(const View& s,const Dependencies& d) {
    const auto& a=s.elements;
    const auto& e=d.elements;
    void* const p=a.context;
    if(s.tags(p)!=nullptr && s.words(p)!=nullptr) {
        const I32 old_depth=a.depth(p);
        for(I32 index=old_depth-1;index>=0;--index) {
            const auto byte=a.tag(p,index);
            const I32 type=byte<128?static_cast<I32>(byte):static_cast<I32>(byte)-256;
            if(type==5) {
                const U32 handle=a.word(p,index);
                if(handle!=0) {
                    e.destroy_string_005e5c20(e.context,handle);
                    e.free_006fa390(e.context,handle);
                }
            } else if(type>=16 && type<=25) {
                const U32 handle=a.word(p,index);
                void* const implementer=a.implementer(p);
                e.destroy_engine_slot6(e.context,implementer,type-16,handle);
            }
            s.clear_word(p,index);
        }
    }
    void* const old_tags=s.tags(p);
    s.set_base(p,0);
    a.set_depth(p,0);
    s.set_capacity(p,0);
    if(old_tags!=nullptr) {
        d.release_array_006fa390(e.context,old_tags);
        s.set_tags(p,nullptr);
    }
    void* const old_words=s.words(p);
    if(old_words!=nullptr) {
        d.release_array_006fa390(e.context,old_words);
        s.set_words(p,nullptr);
    }
}
}
