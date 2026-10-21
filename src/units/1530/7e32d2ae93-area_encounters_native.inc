struct NativeAreaEncounters final:AreaEncounterPorts{
 Word list(Word a,Word b,Word c,Word d)override{if(d!=0x7474c8)required(d);return gff_list_004118c0(ptr(a),nullptr,ptr(b),ptr(c),"Encounter List");}
 Word count(Word a,Word b)override{return gff_list_count_00411940(ptr(a),nullptr,ptr(b));}
 Word nonempty_005050ba(Word,Word,Word,Word,Word,Word)override{required(0x5050ba);}
};
