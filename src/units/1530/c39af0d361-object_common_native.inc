// closeB (Claude): private shadow copy of st/source/object_common_native.inc (effects_nonempty bound).
const char* object_common_label(Word a){switch(a){case 0x745eac:return "EffectList";case 0x745ea0:return "ActionList";case 0x745f08:return "Commandable";default:required(a);}}
struct NativeObjectCommon:ObjectCommonPorts{
 Word list(Word a,Word b,Word c,Word d)override{return gff_list_004118c0(ptr(a),nullptr,ptr(b),ptr(c),object_common_label(d));}
 Word count(Word a,Word b)override{return gff_list_count_00411940(ptr(a),nullptr,ptr(b));}
 void effects_nonempty(Word a,Word b,Word,Word s)override{closeB_object_load_effect_list_tail_004d1c36(ptr(a),nullptr,ptr(b),ptr(s+8));/* closeB */}
 void actions_nonempty(Word a,Word b,Word,Word s)override{loadgame_object_load_action_queue_tail_004ced20(ptr(a),nullptr,ptr(b),ptr(s+0xb4));/* loadgame */}
 void variables(Word a,Word b,Word c)override{native_local_variables(a,b,c);}
 void builtins(Word a,Word b,Word c)override{native_builtin_variables(a,b,c);}
 Word byte_read(Word a,Word b,Word c,Word d,Word e)override{return gui_load_gff_byte_00411a60(ptr(a),nullptr,ptr(b),object_common_label(c),static_cast<unsigned*>(ptr(d)),static_cast<unsigned char>(e));}
};
void native_object_common(Word a,Word b,Word c){NativeMemory m;NativeObjectCommon p;alignas(8) unsigned char s[0xd0];object_common_004d1cf0(m,p,a,b,c,word(s));}
