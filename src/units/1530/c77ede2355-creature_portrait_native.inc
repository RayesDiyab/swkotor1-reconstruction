struct NativeCreaturePortrait:CreaturePortraitPorts,CreaturePortraitRefPorts{
 void literal(Word a,Word b)override{const char*t;switch(b){case 0x745e4c:t="po_";break;case 0x745e40:t="BaseResRef";break;default:required(b);}frame_LogStringCtor_005e5a90(ptr(a),nullptr,t);}
 void empty(Word a)override{*static_cast<unsigned*>(ptr(a))=0;*static_cast<unsigned*>(ptr(a+4))=0;}
 void ref_literal(Word a,Word b)override{(void)static_cast<menu_resources::Ref*>(ptr(a))->construct_00406d80(b==0x73d71c?"":static_cast<const char*>(ptr(b)));}
 void set_ref(Word a,Word b,Word c)override{using F=void(__fastcall*)(void*,void*,void*);reinterpret_cast<F>(a)(ptr(b),nullptr,ptr(c));}
 Word table_string(Word a,Word b,Word c,Word d)override{return c2da_current_get_string_00413270(ptr(a),nullptr,static_cast<int>(b),ptr(c),ptr(d));}
 void assign(Word a,Word b)override{(void)async_assign_string_005e5c50(ptr(a),nullptr,ptr(b));}
 Word text(Word a)override{return word(const_cast<char*>(WindowCStr005e5670(ptr(a),nullptr)));}
 void destroy(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
 void ref_assign(Word a,Word b,Word c)override{(void)resource_ref_assignment::assign_00406120(static_cast<menu_resources::Ref*>(ptr(a)),nullptr,static_cast<menu_resources::Ref*>(ptr(b)),static_cast<const menu_resources::Ref*>(ptr(c)));}
};
extern "C" void __fastcall recovered_creature_portrait_004ed4c0(void*self,void*,unsigned value){NativeMemory m;NativeCreaturePortrait p;alignas(8) unsigned char s[0x28];creature_portrait_004ed4c0(m,p,word(self),value,word(s));}
extern "C" void __fastcall recovered_creature_portrait_ref_004f83c0(void*self,void*,void*ref){NativeMemory m;NativeCreaturePortrait p;alignas(8) unsigned char s[0x20];creature_portrait_ref_004f83c0(m,p,word(self),word(ref),word(s));}
