extern "C" void __fastcall recovered_effect_copy_00504090(void*,void*,void*,unsigned);
struct NativePropertyMemory:NativeMemory{Word read(Word a)override{switch(a){case 0x7468c8:return 1;case 0x7468cc:return 2;case 0x7468d0:return 3;case 0x7468d4:return 4;case 0x7468d8:return 5;case 0x7468dc:return 7;default:return NativeMemory::read(a);}}Word read16(Word a)override{if(a==0x7466a8)return 11;if(a==0x7473bc)return 3;return NativeMemory::read16(a);}};
struct NativePropertyEffect:PropertyEffectPorts{NativeMemory&m;explicit NativePropertyEffect(NativeMemory&v):m(v){}
Word allocate(Word a)override{return word(::operator new(a));}Word construct(Word a,Word b)override{return native_effect_construct(a,b);}
void string(Word a,Word b)override{if(b!=0x744bfc)required(b);frame_LogStringCtor_005e5a90(ptr(a),nullptr,"Value");}
Word table(Word a,Word b)override{return property_cost_table_005c49a0(m,a,b);}
void integer_read(Word a,Word b,Word c,Word d)override{(void)c2da_current_get_int_00413660(ptr(a),nullptr,b,ptr(c),static_cast<unsigned*>(ptr(d)));}
void destroy(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}void creator(Word a,Word b)override{native_effect_creator(a,b);}void integer(Word a,Word b,Word c)override{effect_integer_005036a0(m,a,b,c);}
Word base(Word a,Word b)override{return word(kotor_m15b_base_item_array_get_base_item_005b31a0(ptr(a),nullptr,static_cast<int>(b)));}
void copy(Word a,Word b,Word c)override{recovered_effect_copy_00504090(ptr(a),nullptr,ptr(b),c);}void apply(Word a,Word b,Word c,Word d)override{native_effect_apply(a,b,c,d);}
};
extern "C" unsigned __fastcall recovered_property_effect_004e60c0(void*,void*,void*item,void*property,void*creature,unsigned slot,unsigned flag){NativePropertyMemory m;NativePropertyEffect p(m);alignas(8)unsigned char scratch[0x14];return property_effect_004e60c0(m,p,word(item),word(property),word(creature),slot,flag,word(scratch));}
