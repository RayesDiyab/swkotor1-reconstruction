extern "C" unsigned __fastcall recovered_localized_different_005ea030(void*,void*,void*);
struct NativeItemStack final:ItemStackCompatiblePorts,ItemStackMergePorts{
 NativeMemory&m;explicit NativeItemStack(NativeMemory&v):m(v){}
 Word string_different(Word a,Word b)override{return !async_string_equal_005e5200(ptr(a),nullptr,ptr(b));}
 Word localized_different(Word a,Word b)override{return recovered_localized_different_005ea030(ptr(a),nullptr,ptr(b));}
 Word property(Word fn,Word a,Word b)override{if(fn!=0x553960&&fn!=0x553990)required(fn);return item_stack_property(m,a,b,fn==0x553990);}
 Word compatible(Word a,Word b)override{return item_stack_compatible_00553cf0(m,*this,a,b);}
 Word base(Word a,Word b)override{return word(kotor_m15b_base_item_array_get_base_item_005b31a0(ptr(a),nullptr,static_cast<int>(b)));}
};
extern "C" unsigned __fastcall recovered_item_stack_merge_00553f90(void*self,void*,void*other){NativeMemory m;NativeItemStack p(m);return item_stack_merge_00553f90(m,p,word(self),word(other));}
