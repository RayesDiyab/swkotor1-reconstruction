// 0886-gates (Claude): private copy; area-property ports bound to real bodies (RET checked).
extern "C" {
struct Claude0886ExoString { unsigned data, length; };
void __fastcall claude_exo_string_list_add_004059a0(void*,void*,Claude0886ExoString);      // 004059A0 CExoArrayList<CExoString>::Add (by value) RET 8 (claude_0886_bodies_vars)
void __fastcall recovered_ref_array_grow_00506e20(void*,void*,unsigned);                   // 00506E20 CExoArrayList<CResRef>::Allocate RET 4 (1141)
void __fastcall kotor_fastcall_00563a80(void*,void*,unsigned);                             // 00563A80 CSWPartyTable::SetPartyFollowState RET 4 (claude_0886_bodies_party)
void __fastcall recovered_unstealth_party_00563c60(void*,void*,unsigned);                  // 00563C60 CSWPartyTable::UnstealthParty RET 4 (w2_party_native)
}
extern "C" void __fastcall recovered_area_map_setup_00578c60(void*,void*,Word,Word,Word,Word,Word,Word,Word,Word,Word,Word,Word,Word);
// Draft typed bridge. Missing providers remain exact-address diagnostic gates.
#include "57e6cdb552-area_property_literals.inc"
struct NativeAreaRoomStorage:AreaRoomStoragePorts{
 Word allocate(Word n)override{return word(::operator new(n));}void free(Word a)override{::operator delete(ptr(a));}
 void construct_string(Word a)override{*static_cast<Word*>(ptr(a))=0;*static_cast<Word*>(ptr(a+4))=0;}void destroy_string(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
};
struct NativeAreaProperties:AreaPropertyPorts{
 NativeMemory&m;explicit NativeAreaProperties(NativeMemory&v):m(v){}
 Word scalar(Word fn,Word a,Word b,Word label,Word d,Word e)override{
 const char*c=area_property_literal(label);switch(fn){
 case 0x411a60:return gui_load_gff_byte_00411a60(ptr(a),nullptr,ptr(b),c,static_cast<unsigned*>(ptr(d)),static_cast<unsigned char>(e));
 case 0x411b40:{NativeGffExtra q(m);return gff_short_00411b40(m,q,a,b,word(const_cast<char*>(c)),d,e);}
 case 0x411c20:return gui_load_gff_dword_00411c20(ptr(a),nullptr,ptr(b),c,static_cast<unsigned*>(ptr(d)),e);
 case 0x411c90:return static_cast<Word>(gui_load_gff_int_00411c90(ptr(a),nullptr,ptr(b),c,static_cast<unsigned*>(ptr(d)),static_cast<int>(e)));
 default:required(fn);}}
 void literal(Word a,Word b)override{frame_LogStringCtor_005e5a90(ptr(a),nullptr,area_property_literal(b));}
 void empty(Word a)override{m.write(a,0);m.write(a+4,0);}void assign(Word a,Word b)override{(void)async_assign_string_005e5c50(ptr(a),nullptr,ptr(b));}void destroy(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
 Word string(Word a,Word b,Word c,Word d,Word e,Word f)override{return word(gui_load_gff_string_00411ec0(ptr(a),nullptr,ptr(b),ptr(c),area_property_literal(d),static_cast<unsigned*>(ptr(e)),ptr(f)));}
 Word lower(Word a,Word b)override{return word(directory_list::LowerCase005e6080(static_cast<S*>(ptr(a)),nullptr,static_cast<S*>(ptr(b))));}
 Word list(Word a,Word b,Word c,Word d)override{return gff_list_004118c0(ptr(a),nullptr,ptr(b),ptr(c),area_property_literal(d));}Word count(Word a,Word b)override{return gff_list_count_00411940(ptr(a),nullptr,ptr(b));}void element(Word a,Word b,Word c,Word d)override{(void)gff_list_element_00411990(ptr(a),nullptr,ptr(b),ptr(c),d);}
 Word allocate(Word n)override{return word(::operator new(n));}void free(Word a)override{::operator delete(ptr(a));}
 Word localized_construct(Word a)override{return word(exo_locstring_construct_005e9e10(ptr(a),nullptr));}
 Word localized(Word a,Word b,Word c,Word d,Word e,Word f)override{NativeLocalized q(m);Word scratch[6];return gff_localized_00411fd0(m,q,a,b,c,word(const_cast<char*>(area_property_literal(d))),e,f,word(scratch));}
 void localized_assign(Word a,Word b)override{NativeLocalized q(m);Word scratch;(void)localized_assign_005e9e70(m,q,a,b,word(&scratch));}void localized_destroy(Word a)override{exo_locstring_destroy_005ea000(ptr(a),nullptr);}void append(Word a,Word b)override{(void)resource_append_005e9dc0(ptr(a),nullptr,ptr(b));}
 void ref_literal(Word a,Word b)override{(void)static_cast<menu_resources::Ref*>(ptr(a))->construct_00406d80(area_property_literal(b));}
 Word ref(Word a,Word b,Word c,Word d,Word e,Word f)override{return word(gui_load_gff_resref_00411e10(ptr(a),nullptr,ptr(b),ptr(c),area_property_literal(d),static_cast<unsigned*>(ptr(e)),ptr(f)));}
 void ref_string(Word a,Word b)override{c2da_ref_to_string_00405f70(ptr(a),nullptr,ptr(b));}
 void ref_assign(Word a,Word b,Word c)override{(void)resource_ref_assignment::assign_00406120(static_cast<menu_resources::Ref*>(ptr(a)),nullptr,static_cast<menu_resources::Ref*>(ptr(b)),static_cast<const menu_resources::Ref*>(ptr(c)));}
 Word float_bits(Word a,Word b,Word c,Word d,Word e)override{float fallback;std::memcpy(&fallback,&e,4);float result=gui_load_gff_float_00411d00(ptr(a),nullptr,ptr(b),area_property_literal(c),static_cast<unsigned*>(ptr(d)),fallback);Word bits;std::memcpy(&bits,&result,4);return bits;}
 void float_store(Word a,Word b,Word c,Word d,Word e,Word destination,bool clamp)override{Word bits=float_bits(a,b,c,d,e);if(clamp)area_fog_store(bits,static_cast<Word*>(ptr(destination)));else m.write(destination,bits);}
 Word ref_nonempty(Word a)override{return m.read8(a)!=0;}
 void ref_assign_literal(Word a,Word b,Word c)override{(void)unresolved_ref_assign_00406290(ptr(a),nullptr,ptr(b),area_property_literal(c));}
 void release(Word a)override{c2da_release_demand_00409b80(ptr(a),nullptr);}
 void rooms_allocate(Word a,Word b)override{NativeAreaRoomStorage q;area_rooms_allocate_00506910(m,q,a,b);}
 void copy_string(Word a,Word b)override{(void)WindowCopy005e5b00(ptr(a),nullptr,ptr(b));}
 void append_owned_string(Word a,Word b)override{Claude0886ExoString v;std::memcpy(&v,ptr(b),8);claude_exo_string_list_add_004059a0(ptr(a),nullptr,v);}void ref_array_grow(Word a,Word b)override{recovered_ref_array_grow_00506e20(ptr(a),nullptr,b);}
 Word global(Word a)override{return m.read(a);}Word module(Word a)override{return word(server_internal_get_module_004b14f0(ptr(m.read(a+4)),nullptr));}Word cstr(Word a)override{return word(const_cast<char*>(WindowCStr005e5670(ptr(a),nullptr)));}
 void format1(Word a,Word b,Word c)override{kotor_reconstruction::StartupFormat005e5680(static_cast<S*>(ptr(a)),area_property_literal(b),ptr(c));}
 void ref_from_string(Word a,Word b)override{(void)async_ref_construct_string_00406d60(ptr(a),nullptr,ptr(b));}
 Word exists(Word a,Word b,Word c,Word d)override{return static_cast<menu_resources::Manager*>(ptr(a))->exists_00408bc0(static_cast<menu_resources::Ref*>(ptr(b)),c,static_cast<unsigned*>(ptr(d)));}
 Word structure(Word a,Word b,Word c,Word d)override{return gui_load_gff_struct_00411a10(ptr(a),nullptr,ptr(b),ptr(c),area_property_literal(d));}
 Word field_type(Word a,Word b,Word c,Word d)override{NativeGffExtra q(m);return gff_field_type_00411880(m,q,a,b,c?word(const_cast<char*>(area_property_literal(c))):0,d);}Word map_float_integer(Word a,Word b,Word c,Word d,Word factor)override{Word bits=float_bits(a,b,c,d,0),result;if(factor!=0x747748&&factor!=0x7455d4)required(factor);float input;std::memcpy(&input,&bits,4);const double scaled=static_cast<double>(input)*(factor==0x747748?440.0:256.0)+0.5;(void)result;return w2::ftol2_006fae8c(::floor(scaled));}void map_setup(Word a,const Word*b)override{recovered_area_map_setup_00578c60(ptr(a),nullptr,b[0],b[1],b[2],b[3],b[4],b[5],b[6],b[7],b[8],b[9],b[10],b[11]);}
 Word party(Word a)override{return word(lane_config_required::GetPartyTable_004aee70(ptr(a),nullptr));}void party_clear(Word a,Word b)override{kotor_fastcall_00563a80(ptr(a),nullptr,b);}void party_restrict(Word a,Word b)override{recovered_unstealth_party_00563c60(ptr(a),nullptr,b);}void byte(Word a,Word b)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(b);}
};
