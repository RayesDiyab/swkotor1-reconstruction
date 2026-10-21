extern "C" unsigned __fastcall recovered_stats_effect_query_005a6960(void*,void*,unsigned,void*);
struct NativeEffectHandler:EffectHandlerPorts{NativeEffectType68 shared;explicit NativeEffectHandler(NativeMemory&m):shared(m){}
 Word creature(Word a)override{return shared.creature(a);}Word integer(Word a,Word b)override{return shared.integer(a,b);}
 Word lookup(Word a,Word b)override{return word(kotor_fastcall_004ae770(ptr(a),nullptr,b));}
 Word protected_from(Word a,Word b,Word c)override{return recovered_stats_effect_query_005a6960(ptr(a),nullptr,b,ptr(c));}
};
extern "C" unsigned __fastcall recovered_effect_handler_004d9170(void*,void*,void*target,void*effect,unsigned mode){NativeMemory m;NativeEffectHandler p(m);return effect_handler_004d9170(m,p,word(target),word(effect),mode);}
