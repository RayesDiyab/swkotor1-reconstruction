// closeB (Claude): private shadow copy (st/source/door_fields_native_finish.inc; mesh_finish -> 00596C10).
#include "745b4c65ac-door_field_literals.inc"
struct NativeDoorFieldMemory:NativeMemory{Word read(Word a)override{if(a==0x74c394)return 0;if(a==0x74c39c)return 2;return NativeMemory::read(a);}};
struct NativeDoorFields final:DoorFieldTailPorts{
 NativeMemory&m;explicit NativeDoorFields(NativeMemory&v):m(v){}
 Word dword(Word a,Word b,Word c,Word d,Word e)override{return gui_load_gff_dword_00411c20(ptr(a),nullptr,ptr(b),door_field_literal(c),static_cast<unsigned*>(ptr(d)),e);}
 Word byte(Word a,Word b,Word c,Word d,Word e)override{return gui_load_gff_byte_00411a60(ptr(a),nullptr,ptr(b),door_field_literal(c),static_cast<unsigned*>(ptr(d)),static_cast<unsigned char>(e));}
 Word short_value(Word a,Word b,Word c,Word d,Word e)override{NativeGffExtra p(m);return gff_signed_short_00411bb0(m,p,a,b,word(const_cast<char*>(door_field_literal(c))),d,e);}
 Word word_value(Word a,Word b,Word c,Word d,Word e)override{NativeGffExtra p(m);return gff_short_00411b40(m,p,a,b,word(const_cast<char*>(door_field_literal(c))),d,e);}
 Word floating(Word a,Word b,Word c,Word d,Word e)override{float fallback;std::memcpy(&fallback,&e,4);float result=gui_load_gff_float_00411d00(ptr(a),nullptr,ptr(b),door_field_literal(c),static_cast<unsigned*>(ptr(d)),fallback);Word bits;std::memcpy(&bits,&result,4);return bits;}
 void write_byte(Word a,Word b)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(b);}
 void state(Word a,Word b)override{using F=void(__fastcall*)(void*,void*,unsigned);reinterpret_cast<F>(m.read(m.read(a)+0x7c))(ptr(a),nullptr,b);}
 Word current_hp(Word a)override{using F=unsigned(__fastcall*)(void*,void*,unsigned);return reinterpret_cast<F>(m.read(m.read(a)+0x9c))(ptr(a),nullptr,0);}
 void hitpoints(Word a,Word b)override{creature_hitpoints_set_004cc280(m,a,b);}
 Word continuation_0058a424(Word a,Word b,Word c,Word d,Word e)override{return door_fields_tail_0058a424(m,*this,a,b,c,d,e);}
 Word string_read(Word a,Word b,Word c,Word d,Word e,Word f)override{return word(gui_load_gff_string_00411ec0(ptr(a),nullptr,ptr(b),ptr(c),door_field_literal(d),static_cast<unsigned*>(ptr(e)),ptr(f)));}
 void assign(Word a,Word b)override{(void)async_assign_string_005e5c50(ptr(a),nullptr,ptr(b));}
 void string_destroy(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
 void tag(Word a,Word b)override{NativeCreatureTag p;unsigned scratch[2];creature_tag_00553040(p,a,b,word(scratch));}
 void ref_empty(Word a)override{(void)static_cast<menu_resources::Ref*>(ptr(a))->construct_00406d80("");}
 void ref_default(Word a)override{(void)c2da_default_ref_00405ed0(ptr(a),nullptr);}
 Word ref_read(Word a,Word b,Word c,Word d,Word e,Word f)override{return word(gui_load_gff_resref_00411e10(ptr(a),nullptr,ptr(b),ptr(c),door_field_literal(d),static_cast<unsigned*>(ptr(e)),ptr(f)));}
 void ref_assign(Word a,Word b,Word c)override{(void)resource_ref_assignment::assign_00406120(static_cast<menu_resources::Ref*>(ptr(a)),nullptr,static_cast<menu_resources::Ref*>(ptr(b)),static_cast<const menu_resources::Ref*>(ptr(c)));}
 Word ref_raw(Word a)override{return word(kotor_m15e_slot00c_self_00641db0(ptr(a),nullptr));}
 void portrait_ref(Word a,Word b)override{using F=void(__fastcall*)(void*,void*,void*);reinterpret_cast<F>(m.read(m.read(a)+0xcc))(ptr(a),nullptr,ptr(b));}
 void portrait_id(Word a,Word b)override{using F=void(__fastcall*)(void*,void*,unsigned);reinterpret_cast<F>(m.read(m.read(a)+0xd4))(ptr(a),nullptr,b);}
 Word localized_construct(Word a)override{return word(exo_locstring_construct_005e9e10(ptr(a),nullptr));}
 Word localized_read(Word a,Word b,Word c,Word d,Word e,Word f)override{NativeLocalized p(m);unsigned scratch[6];return gff_localized_00411fd0(m,p,a,b,c,word(const_cast<char*>(door_field_literal(d))),e,f,word(scratch));}
 void localized_assign(Word a,Word b)override{NativeLocalized p(m);unsigned scratch;(void)localized_assign_005e9e70(m,p,a,b,word(&scratch));}
 void localized_destroy(Word a)override{exo_locstring_destroy_005ea000(ptr(a),nullptr);}
 Word ref_from_string(Word a,Word b)override{return word(async_ref_construct_string_00406d60(ptr(a),nullptr,ptr(b)));}
 Word ref_text(Word a)override{return word(c2da_ref_00405fe0(ptr(a),nullptr));}
 void string_literal(Word a,Word b)override{frame_LogStringCtor_005e5a90(ptr(a),nullptr,door_field_literal(b));}
 void string_text(Word a,Word b)override{frame_LogStringCtor_005e5a90(ptr(a),nullptr,static_cast<const char*>(ptr(b)));}
 void string_zero(Word a)override{m.write(a,0);m.write(a+4,0);}
 Word string_empty(Word a)override{return frame_LogIsEmpty_005e58a0(ptr(a),nullptr);}
 Word string_equal_literal(Word a,Word b)override{return static_cast<kotor_reconstruction::StartupString*>(ptr(a))->BorrowedEquals005e5290(door_field_literal(b));}
 void table_string(Word a,Word b,Word c,Word d)override{(void)c2da_current_get_string_00413270(ptr(a),nullptr,static_cast<int>(b),ptr(c),ptr(d));}
 void table_integer(Word a,Word b,Word c,Word d)override{(void)c2da_current_get_int_00413660(ptr(a),nullptr,b,ptr(c),static_cast<unsigned*>(ptr(d)));}
 void assign_literal(Word a,Word b)override{GuiAssign005e5140(ptr(a),nullptr,static_cast<const char*>(ptr(b)));}
 Word allocate(Word a)override{return word(::operator new(a));}
 Word mesh_construct(Word a)override{return native_door_mesh_construct(a);}void mesh_state(Word a,Word b)override{native_door_mesh_state(a,b);}
 void mesh_load(Word a,Word b)override{NativeRoomMeshLoad p(m);unsigned char scratch[0x30];(void)room_mesh_load_00596670(m,p,a,b,word(scratch));}
 void mesh_finish(Word a)override{kotor_fastcall_00596c10(ptr(a),nullptr);/* closeB: 0058B373 full ComputeMeshNormals */}
 void finish(Word a)override{native_door_finish(a);}
};
Word native_door_fields(Word a,Word b,Word c,Word d){NativeDoorFieldMemory m;NativeDoorFields p(m);alignas(8)unsigned char scratch[0xa0];return door_fields_0058a1f0(m,p,a,b,c,d,word(scratch));}
