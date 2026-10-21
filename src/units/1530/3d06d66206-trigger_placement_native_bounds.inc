// 0886-gates (Claude): private copy; RemoveObjectFromArea / CExoArrayList<int>::remove bound (RET 4 each).
extern "C" {
unsigned __fastcall kotor_fastcall_0050dc40(void*,void*,unsigned);            // 0050DC40 (claude_combat_spell_mine)
void __fastcall recovered_int_list_remove_0066dcc0(void*,void*,unsigned);     // 0066DCC0 (w1_native_b7)
}
struct NativeTriggerPlacement final:TriggerPlacementPorts,recovered_creature_area_access::Ports{
 NativeMemory&m;explicit NativeTriggerPlacement(NativeMemory&v):m(v){}
 Word read(Word a)override{return m.read(a);}void write(Word a,Word b)override{m.write(a,b);}Word invalid()override{return 0x7f000000;}
 Word area(Word a)override{return word(recovered_object_area_004cb120(ptr(a),nullptr));}
 void set_area(Word a,Word b)override{recovered_creature_area_access::set(a,b,*this);}
 void remove(Word a,Word b)override{(void)kotor_fastcall_0050dc40(ptr(a),nullptr,b);}void erase(Word a,Word b)override{recovered_int_list_remove_0066dcc0(ptr(a),nullptr,b);}
 void position(Word a,Word b,Word c,Word d,Word e)override{native_object_position(a,b,c,d,e);}
 void string_zero(Word a)override{m.write(a,0);m.write(a+4,0);}void string_destroy(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
 void insert(Word a,Word b,Word c)override{(void)recovered_area_insert_0050dfd0(ptr(a),nullptr,b,c);}
 void grow(Word a,Word b)override{menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(ptr(a)),nullptr,b);}
 void bounds(Word a,Word b,Word c,Word d,Word e)override{recovered_trigger_bounds_0058cc60(ptr(a),nullptr,ptr(b),ptr(c),ptr(d),ptr(e));}
};
void native_trigger_placement(Word a,Word b,Word c,Word d,Word e,Word f){NativeMemory m;NativeTriggerPlacement p(m);alignas(8)unsigned char scratch[0x54];trigger_placement_0058f030(m,p,a,b,c,d,e,f,word(scratch));}
