extern "C" void __fastcall recovered_stats_level_history_005b0d49(void*,void*,void*,void*,void*);
struct NativeCreatureClasses:CreatureStatsClassPorts,CreatureStatsSavePorts,CreatureClassLeafPorts{
 NativeMemory&m;explicit NativeCreatureClasses(NativeMemory&v):m(v){}
 Word list(Word a,Word b,Word c,Word d)override{return gff_list_004118c0(ptr(a),nullptr,ptr(b),ptr(c),creature_stats_literal(d));}
 Word count(Word a,Word b)override{return gff_list_count_00411940(ptr(a),nullptr,ptr(b));}
 Word element(Word a,Word b,Word c,Word d)override{return gff_list_element_00411990(ptr(a),nullptr,ptr(b),ptr(c),d);}
 Word integer_read(Word a,Word b,Word c,Word d,Word e)override{return gui_load_gff_int_00411c90(ptr(a),nullptr,ptr(b),creature_stats_literal(c),static_cast<unsigned*>(ptr(d)),e);}
 Word short_read(Word a,Word b,Word c,Word d,Word e)override{NativeGffExtra p(m);return gff_signed_short_00411bb0(m,p,a,b,word(const_cast<char*>(creature_stats_literal(c))),d,e);}
 Word byte_read(Word a,Word b,Word c,Word d,Word e)override{return gui_load_gff_byte_00411a60(ptr(a),nullptr,ptr(b),creature_stats_literal(c),static_cast<unsigned*>(ptr(d)),static_cast<unsigned char>(e));}
 void byte_write(Word a,Word b)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(b);}
 void set_class(Word a,Word b,Word c)override{creature_set_class_005a5420(m,*this,a,b,c);}
 Word get_class(Word a,Word b)override{return creature_get_class_005a4e90(m,a,b);}
 Word force_class(Word a)override{return creature_force_class_005be4a0(a);}
 void loc_destroy(Word a)override{exo_locstring_destroy_005ea000(ptr(a),nullptr);}
 Word continuation_005b0cdb(Word a,Word b,Word c,Word d,Word e)override{return creature_stats_saves_005b0cdb(m,*this,a,b,c,d,e);}
 void level_history_005b0d49(Word a,Word b,Word c,Word d)override{recovered_stats_level_history_005b0d49(ptr(a),nullptr,ptr(b),ptr(c),ptr(d));}
 Word continuation_005b1254(Word a,Word b,Word c,Word d,Word e)override{NativeCreatureHealth p(m);return creature_stats_health_005b1254(m,p,a,b,c,d,e);}
};
