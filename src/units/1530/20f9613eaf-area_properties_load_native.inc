extern "C" unsigned __fastcall recovered_area_audio_load_005c95f0(void*,void*,void*,void*);
struct NativeAreaPropertiesLoad final:recovered_area_properties_load::Ports{
 NativeMemory&m;NativeAreaProperties base;explicit NativeAreaPropertiesLoad(NativeMemory&v):m(v),base(v){}
 Word read(Word a)override{return m.read(a);}void write(Word a,Word b)override{m.write(a,b);}Word byte(Word a)override{return *static_cast<unsigned char*>(ptr(a));}void byte(Word a,Word b)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(b);}
 Word structure(Word a,Word b,Word c,Word d)override{if(d!=0x747660)required(d);return gui_load_gff_struct_00411a10(ptr(a),nullptr,ptr(b),ptr(c),"AreaProperties");}
 Word scalar(Word fn,Word a,Word b,Word c,Word d,Word e)override{return base.scalar(fn,a,b,c,d,e);}Word party()override{return base.party(m.read(m.read(0x7a39fc)+8));}void restrict_party(Word a,Word b)override{base.party_restrict(a,b);}Word child(Word a,Word b,Word c)override{return recovered_area_audio_load_005c95f0(ptr(a),nullptr,ptr(b),ptr(c));}
};
unsigned native_area_properties_load(Word a,Word b,Word c){NativeMemory m;NativeAreaPropertiesLoad p(m);unsigned scratch[2];return recovered_area_properties_load::load(a,b,c,word(scratch),p);}
