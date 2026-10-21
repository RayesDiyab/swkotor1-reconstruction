// 0886-gates: 005AF4E9 decreased-XP arm: [0074AD8C] = "<NO TAG>", [0074AD4C] = the format literal; the original pointer
// compare of CStr() against the "" literal 0073D71C binds to CStr() of the empty scratch CExoString (same sentinel).
// (kotor_movie::string_element_construct_005b3190 / kotor_reconstruction::StartupFormat005e5680 are declared at global scope in the cell)
extern "C" void* __fastcall async_string_cstr_005e5670(void*,void*);                                  // 005E5670 CExoString::CStr (0188)
// 0886-gates (Claude): private copy of the level-history-t2 overlay; ability effect bonus bound to the complete 004F3FE0.
extern "C" {
unsigned __fastcall kotor_fastcall_004f3fe0(void*,void*,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned); // 004F3FE0 GetTotalEffectBonus RET 28h (claude_combat_attack_effectbonus, complete)
}
extern "C" char* __fastcall c2da_ref_00405fe0(void*,void*) noexcept;
extern "C" int __fastcall c2da_current_get_float_00413430(void*,void*,unsigned,void*,float*);
#include "2d6cf31ecc-creature_stats_literals.inc"
#include "5b18f2abe9-creature_add_feat_native.inc"
#include "e157d6d7cf-creature_forcepoints_native_combatguard.inc"
#include "afe64ebd37-creature_stats_health_native_feats.inc"
#include "5a8c8426a4-creature_stats_class_native_health.inc"
struct NativeCreatureTag:CreatureTagPorts{Word lower(Word a,Word b)override{return word(directory_list::LowerCase005e6080(static_cast<kotor_reconstruction::StartupString*>(ptr(a)),nullptr,static_cast<kotor_reconstruction::StartupString*>(ptr(b))));}void assign(Word a,Word b)override{(void)async_assign_string_005e5c50(ptr(a),nullptr,ptr(b));}void destroy(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}};
struct NativeCreatureAbility:CreatureAbilityPorts,CreatureEffectAbilityPorts{NativeMemory&m;explicit NativeCreatureAbility(NativeMemory&v):m(v){}Word effects(Word a,Word b,Word c,Word d,Word e,Word f,Word g,Word h,Word i,Word j,Word k)override{return kotor_fastcall_004f3fe0(ptr(a),nullptr,b,c,d,e,f,g,h,i,j,k);}Word nonempty(Word a,Word b,Word c)override{return kotor_fastcall_004f3fe0(ptr(a),nullptr,4,0,b,0,0,0,0,0xff,c,0);}Word force_class(Word a)override{return creature_force_class_005be4a0(a);}Word slots(Word a,Word b,Word c)override{return creature_force_slots_005a5ee0(m,a,b,c);}void byte_write(Word a,Word b)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(b);}};
extern "C" void* __fastcall kotor_fastcall_004ae910(void*,void*,unsigned);
struct NativeCreatureOwnerValues:CreatureOwnerValuePorts{NativeMemory&m;explicit NativeCreatureOwnerValues(NativeMemory&v):m(v){}Word party(Word a)override{return m.read(a+4)+0x1b770;}Word player(Word a,Word b)override{return word(kotor_fastcall_004ae910(ptr(a),nullptr,b));}Word message(Word a)override{return m.read(m.read(a+4)+0x10010);}void empty(Word a)override{m.write(a,0);m.write(a+4,0);}void destroy(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}void decreased_experience(Word a,Word b,Word c)override{unsigned char text[8];(void)kotor_movie::string_element_construct_005b3190(text,nullptr);const char*name="<NO TAG>";const Word creature=m.read(a+0x24);if(creature&&async_string_cstr_005e5670(ptr(creature+0x18),nullptr)!=async_string_cstr_005e5670(ptr(c+0x18),nullptr))name=static_cast<const char*>(async_string_cstr_005e5670(ptr(creature+0x18),nullptr));kotor_reconstruction::StartupFormat005e5680(reinterpret_cast<kotor_reconstruction::StartupString*>(text),"Tried to reduce XP of creature '%s' to '%d'. Cannot reduce XP.",name,static_cast<int>(b));frame_LogStringDtor_005e5c20(text,nullptr);}void notify_experience(Word a,Word b,Word c,Word d)override{closeB_stats_experience_notify_005af555(ptr(a),nullptr,b,c,ptr(d));/* closeB: 005AF555..005AF678 (claude_closeB_xp) */}};
struct NativeCreatureStats:CreatureStatsScalarPorts,CreatureStatsAppearancePorts,CreatureStatsPerceptionPorts {
 NativeMemory&m;explicit NativeCreatureStats(NativeMemory&v):m(v){}
 Word loc_construct(Word a)override{return word(exo_locstring_construct_005e9e10(ptr(a),nullptr));}
 Word loc_read(Word a,Word b,Word c,Word d,Word e,Word f)override{NativeLocalized p(m);unsigned scratch[6];return gff_localized_00411fd0(m,p,a,b,c,word(const_cast<char*>(creature_stats_literal(d))),e,f,word(scratch));}
 void loc_assign(Word a,Word b)override{NativeLocalized p(m);unsigned scratch;(void)localized_assign_005e9e70(m,p,a,b,word(&scratch));}
 void loc_destroy(Word a)override{exo_locstring_destroy_005ea000(ptr(a),nullptr);}
 Word string_read(Word a,Word b,Word c,Word d,Word e,Word f)override{return word(gui_load_gff_string_00411ec0(ptr(a),nullptr,ptr(b),ptr(c),creature_stats_literal(d),static_cast<unsigned*>(ptr(e)),ptr(f)));}
 void set_tag(Word a,Word b)override{NativeCreatureTag p;unsigned scratch[2];creature_tag_00553040(p,a,b,word(scratch));}
 void string_destroy(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
 Word ref_read(Word a,Word b,Word c,Word d,Word e,Word f)override{return word(gui_load_gff_resref_00411e10(ptr(a),nullptr,ptr(b),ptr(c),creature_stats_literal(d),static_cast<unsigned*>(ptr(e)),ptr(f)));}
 void ref_assign(Word a,Word b,Word c)override{(void)resource_ref_assignment::assign_00406120(static_cast<menu_resources::Ref*>(ptr(a)),nullptr,static_cast<menu_resources::Ref*>(ptr(b)),static_cast<const menu_resources::Ref*>(ptr(c)));}
 Word byte_read(Word a,Word b,Word c,Word d,Word e)override{return gui_load_gff_byte_00411a60(ptr(a),nullptr,ptr(b),creature_stats_literal(c),static_cast<unsigned*>(ptr(d)),static_cast<unsigned char>(e));}
 Word integer_read(Word a,Word b,Word c,Word d,Word e)override{return gui_load_gff_int_00411c90(ptr(a),nullptr,ptr(b),creature_stats_literal(c),static_cast<unsigned*>(ptr(d)),e);}
 void byte_write(Word a,Word b)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(b);}
 void string_assign(Word a,Word b)override{(void)async_assign_string_005e5c50(ptr(a),nullptr,ptr(b));}
 Word ability(Word a,Word b)override{Word kind;switch(a){case 0x5a6190:kind=0;break;case 0x5a6550:kind=1;break;case 0x5a6250:kind=2;break;case 0x5a6310:kind=3;break;case 0x5a63d0:kind=4;break;case 0x5a6490:kind=5;break;default:required(a);}NativeCreatureAbility p(m);unsigned scratch;return creature_ability(m,p,b,kind,word(&scratch));}
 void recalculate_005a6d80(Word a)override{NativeCreatureAbility p(m);alignas(8) unsigned char scratch[0x10];creature_recalculate_005a6d80(m,p,a,word(scratch));}
 Word continuation_005affbe(Word a,Word b,Word c,Word d,Word e)override{return creature_stats_scalars_005affbe(m,*this,a,b,c,d,e);}
 Word short_read(Word a,Word b,Word c,Word d,Word e)override{NativeGffExtra p(m);return gff_short_00411b40(m,p,a,b,word(const_cast<char*>(creature_stats_literal(c))),d,e);}
 Word dword_read(Word a,Word b,Word c,Word d,Word e)override{return gui_load_gff_dword_00411c20(ptr(a),nullptr,ptr(b),creature_stats_literal(c),static_cast<unsigned*>(ptr(d)),e);}
 Word owner_value(Word a)override{NativeCreatureOwnerValues p(m);return creature_owner_value_004edd60(m,p,a);}
 void owner_set(Word a,Word b)override{NativeCreatureOwnerValues p(m);creature_owner_set_004edd90(m,p,a,b);}
 void set_experience(Word a,Word b,Word c)override{NativeCreatureOwnerValues p(m);alignas(8) unsigned char scratch[0x20];creature_experience_005af480(m,p,a,b,c,word(scratch));}
 Word continuation_005b024c(Word a,Word b,Word c,Word d,Word e,Word f)override{return creature_stats_owner_005b024c(m,*this,a,b,c,d,e,f);}
 Word continuation_005b03b4(Word a,Word b,Word c,Word d,Word e)override{return creature_stats_appearance_005b03b4(m,*this,a,b,c,d,e);}
 void portrait(Word a,Word b,Word c)override{using F=void(__fastcall*)(void*,void*,unsigned);reinterpret_cast<F>(a)(ptr(b),nullptr,c);}
 void ref_text(Word a)override{(void)c2da_ref_00405fe0(ptr(a),nullptr);}
 Word float_bits(Word a,Word b,Word c,Word d,Word e)override{float fallback;std::memcpy(&fallback,&e,4);float result=gui_load_gff_float_00411d00(ptr(a),nullptr,ptr(b),creature_stats_literal(c),static_cast<unsigned*>(ptr(d)),fallback);Word bits;std::memcpy(&bits,&result,4);return bits;}
 Word continuation_005b06b8(Word a,Word b,Word c,Word d,Word e)override{return creature_stats_perception_005b06b8(m,*this,a,b,c,d,e);}
 void string_literal(Word a,Word b)override{frame_LogStringCtor_005e5a90(ptr(a),nullptr,creature_stats_literal(b));}
 Word table_float(Word a,Word b,Word c,Word d)override{return c2da_current_get_float_00413430(ptr(a),nullptr,b,ptr(c),static_cast<float*>(ptr(d)));}
 Word table_integer(Word a,Word b,Word c,Word d)override{return c2da_current_get_int_00413660(ptr(a),nullptr,b,ptr(c),static_cast<unsigned*>(ptr(d)));}
 Word continuation_005b098b(Word a,Word b,Word c,Word d,Word e)override{NativeCreatureClasses p(m);return creature_stats_classes_005b098b(m,p,a,b,c,d,e);}
};
