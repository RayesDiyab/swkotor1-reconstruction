struct NativeAreaStores final:AreaStorePorts{
 Word list(Word a,Word b,Word c,Word d)override{if(d!=0x747510)required(d);return gff_list_004118c0(ptr(a),nullptr,ptr(b),ptr(c),"StoreList");}
 Word count(Word a,Word b)override{return gff_list_count_00411940(ptr(a),nullptr,ptr(b));}
 Word nonempty_005057fa(Word,Word,Word,Word,Word,Word)override{required(0x5057fa);}
};
