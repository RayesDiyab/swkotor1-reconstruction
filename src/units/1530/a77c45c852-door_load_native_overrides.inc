struct NativeDoorTemplate final:DoorTemplatePorts{
 NativeMemory&m;explicit NativeDoorTemplate(NativeMemory&v):m(v){}
 Word allocate(Word a)override{return word(::operator new(a));}
 Word construct_gff(Word a,Word b,Word c,Word d)override{if(c!=0x7499a8)required(c);return word(gui_load_gff_construct_00410630(static_cast<kotor_q05::CResGFF*>(ptr(a)),nullptr,b,"UTD ",static_cast<const menu_resources::Ref*>(ptr(d))));}
 void root(Word a,Word b)override{(void)static_cast<kotor_q05::CResGFF*>(ptr(a))->GetTopLevelStruct(static_cast<kotor_q05::StructHandle*>(ptr(b)));}
 Word fields(Word a,Word b,Word c,Word d)override{return native_door_fields(a,b,c,d);}
 void destroy(Word a)override{using F=void*(__thiscall*)(void*,unsigned);(void)reinterpret_cast<F>(m.read(m.read(a)))(ptr(a),1);}
};
struct NativeDoorLoad final:DoorLoadPorts{
 NativeMemory&m;explicit NativeDoorLoad(NativeMemory&v):m(v){}
 void ref_empty(Word a)override{(void)static_cast<menu_resources::Ref*>(ptr(a))->construct_00406d80("");}
 void ref_read(Word a,Word b,Word c,Word label,Word status,Word fallback)override{if(label!=0x747494&&label!=0x747ac8)required(label);(void)gui_load_gff_resref_00411e10(ptr(a),nullptr,ptr(b),ptr(c),door_field_literal(label),static_cast<unsigned*>(ptr(status)),ptr(fallback));}
 Word load_template(Word a,Word b)override{NativeDoorTemplate p(m);unsigned root;return door_template_0058b3d0(m,p,a,b,word(&root));}
 Word localized(Word a,Word b,Word c,Word d,Word e,Word f)override{NativeDoorFields p(m);return p.localized_read(a,b,c,d,e,f);}
 void localized_assign(Word a,Word b)override{NativeDoorFields p(m);p.localized_assign(a,b);}void localized_destroy(Word a)override{exo_locstring_destroy_005ea000(ptr(a),nullptr);}
 void string_empty(Word a)override{frame_LogStringCtor_005e5a90(ptr(a),nullptr,"");}
 Word string_read(Word a,Word b,Word c,Word d,Word e,Word f)override{NativeDoorFields p(m);return p.string_read(a,b,c,d,e,f);}
 void assign(Word a,Word b)override{(void)async_assign_string_005e5c50(ptr(a),nullptr,ptr(b));}void destroy(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
 Word byte_read(Word a,Word b,Word c,Word d)override{return gui_load_gff_byte_00411a60(ptr(a),nullptr,ptr(b),door_field_literal(c),static_cast<unsigned*>(ptr(d)),0);}
 void byte_write(Word a,Word b)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(b);}void string_zero(Word a)override{m.write(a,0);m.write(a+4,0);}
 void ref_string(Word a,Word b)override{c2da_ref_to_string_00405f70(ptr(a),nullptr,ptr(b));}
 Word fields(Word a,Word b,Word c,Word d)override{return native_door_fields(a,b,c,d);}
 Word module(Word a)override{return word(server_internal_get_module_004b14f0(ptr(m.read(a+4)),nullptr));}
 void tag(Word a,Word b,Word c)override{(void)recovered_module_register_tag_004c7de0(ptr(a),nullptr,ptr(b),c);}

};
