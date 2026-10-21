struct NativeAreaEffectObjects final:AreaEffectObjectPorts{
 Word list(Word a,Word b,Word c,Word d)override{if(d!=0x74751c)required(d);return gff_list_004118c0(ptr(a),nullptr,ptr(b),ptr(c),"AreaEffectList");}
 Word count(Word a,Word b)override{return gff_list_count_00411940(ptr(a),nullptr,ptr(b));}
 Word nonempty_00505b4a(Word,Word,Word,Word,Word,Word)override{required(0x505b4a);}
};
