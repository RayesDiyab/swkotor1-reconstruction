const char* creature_template_label(Word a){switch(a){case 0x747260:return "PM_IsDisguised";case 0x747250:return "PM_Appearance";case 0x7472d0:return "StealthMode";case 0x745d80:return "XPosition";case 0x745d74:return "YPosition";case 0x745d68:return "ZPosition";case 0x745d38:return "XOrientation";case 0x745d48:return "YOrientation";case 0x745d58:return "ZOrientation";case 0x7472e8:return "JoiningXP";default:required(a);}}
extern "C" void __fastcall recovered_creature_inventory_004ffda0(void*,void*,void*,void*,unsigned,unsigned,unsigned);
extern "C" void __fastcall recovered_creature_scripts_004ebf20(void*,void*,void*,void*);
extern "C" char* __fastcall c2da_ref_00405fe0(void*,void*) noexcept;
extern "C" void __cdecl frame_LogFormat_005e5680(::kotor_reconstruction::StartupString*,const char*,...);
extern "C" void __fastcall recovered_set_stealth_mode_0050ee50(void*,void*,unsigned,unsigned);
struct NativeCreatureTemplate:CreatureTemplatePorts{
 NativeMemory&m;explicit NativeCreatureTemplate(NativeMemory&v):m(v){}
 Word allocate(Word a)override{return word(::operator new(a));}
 Word gff_construct(Word a,Word b,Word c,Word d)override{if(c!=0x747368)required(c);return word(gui_load_gff_construct_00410630(static_cast<kotor_q05::CResGFF*>(ptr(a)),nullptr,b,"UTC ",static_cast<const menu_resources::Ref*>(ptr(d))));}
 void destroy(Word a,Word b)override{using F=void*(__thiscall*)(void*,Word);(void)reinterpret_cast<F>(m.read(m.read(a)))(ptr(a),b);}
 void empty(Word a)override{m.write(a,0);m.write(a+4,0);}
 Word ref_text(Word a)override{return word(c2da_ref_00405fe0(ptr(a),nullptr));}
 void format(Word a,Word,Word c)override{frame_LogFormat_005e5680(static_cast<::kotor_reconstruction::StartupString*>(ptr(a)),"Creature template '%s' doesn't exist.\n",ptr(c));}
 void ref_literal(Word a,Word)override{(void)static_cast<menu_resources::Ref*>(ptr(a))->construct_00406d80("NW_BADGER");}
 void string_destroy(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
 void root(Word a,Word b)override{(void)static_cast<kotor_q05::CResGFF*>(ptr(a))->GetTopLevelStruct(static_cast<kotor_q05::StructHandle*>(ptr(b)));}
 Word stats_header(Word a,Word b,Word c,Word d)override{NativeCreatureStats p(m);alignas(8) unsigned char scratch[0x120];return creature_stats_identity_005afce0(m,static_cast<CreatureStatsScalarPorts&>(p),a,b,c,d,word(scratch));}
 void appearance(Word a,Word b,Word c)override{recovered_creature_scripts_004ebf20(ptr(a),nullptr,ptr(b),ptr(c));}
 void fields(Word a,Word b,Word c,Word d,Word e,Word f)override{recovered_creature_inventory_004ffda0(ptr(a),nullptr,ptr(b),ptr(c),d,e,f);}
 void stats(Word a,Word b,Word c)override{native_creature_known_spells(a,b,c);}
 Word byte(Word a,Word b,Word c,Word d,Word e)override{return gui_load_gff_byte_00411a60(ptr(a),nullptr,ptr(b),creature_template_label(c),static_cast<unsigned*>(ptr(d)),static_cast<unsigned char>(e));}
 Word short_value(Word a,Word b,Word c,Word d,Word e)override{NativeGffExtra p(m);return gff_short_00411b40(m,p,a,b,word(const_cast<char*>(creature_template_label(c))),d,e);}
 void stealth(Word a,Word b,Word c)override{recovered_set_stealth_mode_0050ee50(ptr(a),nullptr,b,c);}
 void object_fields(Word a,Word b,Word c)override{native_object_common(a,b,c);}
 Word float_bits(Word a,Word b,Word c,Word d,Word e)override{float fallback;std::memcpy(&fallback,&e,4);float result=gui_load_gff_float_00411d00(ptr(a),nullptr,ptr(b),creature_template_label(c),static_cast<unsigned*>(ptr(d)),fallback);Word bits;std::memcpy(&bits,&result,4);return bits;}
 Word integer(Word a,Word b,Word c,Word d,Word e)override{return gui_load_gff_int_00411c90(ptr(a),nullptr,ptr(b),creature_template_label(c),static_cast<unsigned*>(ptr(d)),static_cast<int>(e));}
 void position(Word a,Word b,Word c,Word d,Word e)override{native_object_position(a,b,c,d,e);}
 bool orientation_nonzero(Word a,Word b,Word c)override{return !object_float_equal_x87(a,0)||!object_float_equal_x87(b,0)||!object_float_equal_x87(c,0);}
 void orientation(Word a,Word b)override{object_orientation_004cd170(m,a,b);}
 void finish(Word a)override{native_creature_template_finish(a);}
};
