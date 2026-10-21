extern "C" void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void*,void*,int);
struct NativeCreatureDamage final:CreatureDamagePorts {
 NativeMemory&m;explicit NativeCreatureDamage(NativeMemory&v):m(v){}
 Word equipped(Word a,Word b)override{return word(recovered_equipment_item_005a4c20(ptr(a),nullptr,b));}
 Word base(Word a,Word b)override{return word(kotor_m15b_base_item_array_get_base_item_005b31a0(ptr(a),nullptr,static_cast<int>(b)));}
 Word rules(Word a)override{Word id=m.read(a+0xc);return base(m.read(m.read(0x7a3a28)+0x34),id);}
 Word feat(Word a,Word b)override{return recovered_stats_has_feat_005a6630(ptr(a),nullptr,b);}
 Word property(Word a,Word b,Word c,Word d)override{return item_property_query_005539c0(m,a,b,c,d);}
 Word positive_scale(Word a,Word factor)override{
  // 005A7A6F..005A7A7D / 005A7B02..005A7B10: fild; fmul [factor]; call __ftol2 (w2 x87 provider)
  if(factor==0x73e9ac)return w2::ftol2_006fae8c(static_cast<double>(static_cast<int>(a))*0.5);
  if(factor==0x73f414)return w2::ftol2_006fae8c(static_cast<double>(static_cast<int>(a))*1.5);
  required(factor);
 }
};
