extern "C" unsigned char __fastcall recovered_stats_level_005a5fd0(void*,void*,unsigned);
struct NativeCreatureHealth:CreatureStatsHealthPorts{
 NativeMemory&m;explicit NativeCreatureHealth(NativeMemory&v):m(v){}
 Word short_read(Word a,Word b,Word c,Word d,Word e)override{NativeGffExtra p(m);return gff_signed_short_00411bb0(m,p,a,b,word(const_cast<char*>(creature_stats_literal(c))),d,e);}
 Word level(Word a,Word b)override{return recovered_stats_level_005a5fd0(ptr(a),nullptr,b);}
 Word hitpoints(Word a,Word b)override{using F=unsigned short(__fastcall*)(void*,void*,unsigned);return reinterpret_cast<F>(m.read(m.read(a)+0x98))(ptr(a),nullptr,b);}
 void set_hitpoints(Word a,Word b)override{creature_hitpoints_set_004cc280(m,a,b);}
 Word forcepoints(Word a)override{NativeCreatureForcepoints p(m);return creature_forcepoints_004fd490(m,p,a);}
 Word continuation_005b1685(Word a,Word b,Word c,Word d,Word e)override{NativeCreatureFeats p(m);return creature_stats_feats_005b1685(m,p,a,b,c,d,e);}
};
