// closeB (Claude): private shadow copy of st/source/area_creatures_native_safe.inc (load_gff bound to LoadCreature).
extern "C" void __fastcall recovered_creature_placement_004fa100(void*,void*,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned);
extern "C" void* __fastcall recovered_server_creature_construct_004f7a10(void*,void*,unsigned,unsigned);
struct NativeAreaCreatures:AreaCreaturePorts {
 NativeMemory&m;explicit NativeAreaCreatures(NativeMemory&v):m(v){}
 Word list(Word a,Word b,Word c,Word d)override{if(d!=0x7458dc)required(d);return gff_list_004118c0(ptr(a),nullptr,ptr(b),ptr(c),"Creature List");}
 Word count(Word a,Word b)override{return gff_list_count_00411940(ptr(a),nullptr,ptr(b));}
 Word element(Word a,Word b,Word c,Word d)override{return gff_list_element_00411990(ptr(a),nullptr,ptr(b),ptr(c),d);}
 Word type(Word a,Word b)override{return static_cast<kotor_q05::CResGFF*>(ptr(a))->GetElementType(static_cast<kotor_q05::StructHandle*>(ptr(b)));}
 Word allocate(Word a)override{return word(::operator new(a));}
 Word construct(Word a,Word b,Word c)override{return word(recovered_server_creature_construct_004f7a10(ptr(a),nullptr,b,c));}
 void ref_literal(Word a,Word b)override{if(b!=0x73d71c)required(b);(void)static_cast<menu_resources::Ref*>(ptr(a))->construct_00406d80("");}
 void gff_ref(Word a,Word b,Word c,Word d,Word e,Word f)override{if(d!=0x747494)required(d);(void)gui_load_gff_resref_00411e10(ptr(a),nullptr,ptr(b),ptr(c),"TemplateResRef",static_cast<unsigned*>(ptr(e)),ptr(f));}
 Word load_template(Word a,Word b,Word c)override{NativeCreatureTemplate p(m);alignas(8) unsigned char scratch[0x50];return creature_template_005026d0(m,p,a,b,c,word(scratch));}
 Word gff_dword(Word a,Word b,Word c,Word d,Word e)override{return gui_load_gff_dword_00411c20(ptr(a),nullptr,ptr(b),load_literal(c),static_cast<unsigned*>(ptr(d)),e);}
 Word load_gff(Word a,Word b,Word c,Word d)override{return kotor_fastcall_00500350(ptr(a),nullptr,ptr(b),ptr(c),d);/* closeB: 00504C1B */}
 void destroy(Word a,Word b)override{using F=void*(__thiscall*)(void*,Word);(void)reinterpret_cast<F>(m.read(m.read(a)))(ptr(a),b);}
 Word gff_float(Word a,Word b,Word c,Word d,Word e)override{float fallback;std::memcpy(&fallback,&e,4);float result=gui_load_gff_float_00411d00(ptr(a),nullptr,ptr(b),creature_template_label(c),static_cast<unsigned*>(ptr(d)),fallback);Word bits;std::memcpy(&bits,&result,4);return bits;}
 Word safe_position(Word a,Word b,Word c,Word d,Word e,Word f)override{return native_area_safe_position(a,b,c,d,e,f);}
 void place(Word a,Word b,Word c,Word d,Word e,Word f,Word g)override{recovered_creature_placement_004fa100(ptr(a),nullptr,b,c,d,e,f,g);}
 void orient(Word a,Word b)override{object_orientation_004cd170(m,a,b);}
};
