#include "6c137a7caef4f99a_stats_effect_query.hpp"
#include <cstring>
extern "C" void* kotor_cell_rules_007a3a28;
namespace{using U=unsigned;struct Native:recovered_stats_effect_query::Ports{U read(U a)override{U v;std::memcpy(&v,reinterpret_cast<void*>(a),4);return v;}U half(U a)override{unsigned short v;std::memcpy(&v,reinterpret_cast<void*>(a),2);return v;}U rules_race()override{return *reinterpret_cast<unsigned char*>(reinterpret_cast<U>(kotor_cell_rules_007a3a28)+0xaa);}U integer(U a,U i)override{return read(read(a+0x34)+i*4);}};}
extern "C" unsigned __fastcall recovered_stats_effect_query_005a6960(void*self,void*,unsigned kind,void*other){Native p;return recovered_stats_effect_query::query(reinterpret_cast<unsigned>(self),kind,reinterpret_cast<unsigned>(other),p);}
