extern "C" {int __fastcall c2da_current_get_string_entry_00413190(void*,void*,int,int,void*);extern unsigned kotor_rules_cell32_007a20d4;extern unsigned kotor_rules_cell32_007a2124;extern unsigned kotor_rules_cell32_007a2110;}
struct NativeCreatureTemplateFinal final:CreatureTemplateFinalPorts {
 void literal(Word a,Word b)override{const char*t;switch(b){case 0x746cd4:t="haslegs";break;case 0x746ccc:t="hasarms";break;case 0x746cc0:t="weaponscale";break;case 0x746cb0:t="PREFATCKDIST";break;default:required(b);}frame_LogStringCtor_005e5a90(ptr(a),nullptr,t);}
 void empty(Word a)override{*static_cast<unsigned*>(ptr(a))=0;*static_cast<unsigned*>(ptr(a+4))=0;}
 void destroy(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
 void assign(Word a,Word b)override{(void)async_assign_string_005e5c50(ptr(a),nullptr,ptr(b));}
 Word named_integer(Word a,Word b,Word c,Word d)override{return c2da_current_get_int_00413660(ptr(a),nullptr,b,ptr(c),static_cast<unsigned*>(ptr(d)));}
 Word integer(Word a,Word b,Word c,Word d)override{return static_cast<Word>(c2da_current_get_int_entry_00413510(ptr(a),nullptr,static_cast<int>(b),static_cast<int>(c),static_cast<int*>(ptr(d))));}
 Word string(Word a,Word b,Word c,Word d)override{return static_cast<Word>(c2da_current_get_string_entry_00413190(ptr(a),nullptr,static_cast<int>(b),static_cast<int>(c),ptr(d)));}
 Word named_float(Word a,Word b,Word c,Word d)override{return c2da_current_get_float_00413430(ptr(a),nullptr,b,ptr(c),static_cast<float*>(ptr(d)));}
 void movement(Word a,Word b)override{NativeMemory m;NativeCreatureMovement p;alignas(8) unsigned char scratch[0x38];creature_movement_rate_005a5680(m,p,a,b,word(scratch));}
};
void native_creature_template_final(Word self,Word movement){NativeMemory m;NativeCreatureTemplateFinal p;alignas(8) unsigned char scratch[0x30];creature_template_final_004ed850(m,p,self,movement,word(scratch));}
