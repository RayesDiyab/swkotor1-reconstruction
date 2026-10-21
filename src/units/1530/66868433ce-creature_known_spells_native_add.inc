extern "C" void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void*,void*,int);
extern "C" void __fastcall recovered_known_spell_add_00649660(void*,void*,unsigned,unsigned);
struct NativeAddKnownSpell:AddKnownSpellPorts{
 Word spell(Word a,Word b)override{return word(kotor_m15b_spell_array_get_spell_0059b6d0(ptr(a),nullptr,static_cast<int>(b)));}
 Word force_class(Word a)override{return creature_force_class_005be4a0(a);}
 void add(Word a,Word b,Word c)override{recovered_known_spell_add_00649660(ptr(a),nullptr,b,c);}
};
struct NativeCreatureKnownSpells:CreatureKnownSpellPorts{NativeMemory&m;explicit NativeCreatureKnownSpells(NativeMemory&v):m(v){}
Word list(Word a,Word b,Word c,Word d)override{return gff_list_004118c0(ptr(a),nullptr,ptr(b),ptr(c),d==0x74535c?"ClassList":static_cast<const char*>(ptr(d)));}
Word count(Word a,Word b)override{return gff_list_count_00411940(ptr(a),nullptr,ptr(b));}
Word element(Word a,Word b,Word c,Word d)override{return gff_list_element_00411990(ptr(a),nullptr,ptr(b),ptr(c),d);}
void empty(Word a)override{m.write(a,0);m.write(a+4,0);}
void format(Word a,Word b,Word c,Word d)override{if(b!=0x73edd0||c!=0x74ac6c)required(b);kotor_reconstruction::StartupFormat005e5680(static_cast<kotor_reconstruction::StartupString*>(ptr(a)),"%s%d","KnownList",d);}
Word text(Word a)override{return reinterpret_cast<Word>(WindowCStr005e5670(ptr(a),nullptr));}
Word short_read(Word a,Word b,Word c,Word d,Word e)override{if(c!=0x74ac64)required(c);NativeGffExtra p(m);return gff_short_00411b40(m,p,a,b,word(const_cast<char*>("Spell")),d,e);}
void add(Word a,Word b,Word c)override{NativeAddKnownSpell p;creature_add_known_spell_005aa9b0(m,p,a,b,c);}
void destroy(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
};
void native_creature_known_spells(Word a,Word b,Word c){NativeMemory m;NativeCreatureKnownSpells p(m);alignas(8) unsigned char s[0x60];creature_known_spells_005aeb30(m,p,a,b,c,word(s));}
