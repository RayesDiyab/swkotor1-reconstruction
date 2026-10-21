#include "b6b2115261-effect_rank.hpp"
#include "9129ece744-effect_rank_numeric.hpp"
extern "C" {unsigned recovered_effect_rank_scratch_008327e0=0;}
struct NativeEffectRank final:EffectRankPorts {
 NativeMemory&m;explicit NativeEffectRank(NativeMemory&v):m(v){}
 Word integer(Word a,Word b)override{return m.read(m.read(a+0x34)+b*4);}
 void logarithm(Word value)override{effect_rank_logarithm(value,&recovered_effect_rank_scratch_008327e0);}
 void string(Word a)override{frame_LogStringCtor_005e5a90(ptr(a),nullptr,"Rank");}
 void destroy(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
 void read_rank(Word table,Word row,Word label,Word out)override{(void)c2da_current_get_int_00413660(ptr(table),nullptr,row,ptr(label),static_cast<unsigned*>(ptr(out)));}
};
extern "C" unsigned __fastcall recovered_effect_rank_00501649(void*current,void*,void*existing,unsigned*currentScore,unsigned*existingScore){NativeMemory m;NativeEffectRank p(m);unsigned strings[4];return effect_rank_00501649(m,p,word(current),word(existing),word(currentScore),word(existingScore),word(strings));}
