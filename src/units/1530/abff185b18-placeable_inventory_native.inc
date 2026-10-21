// 0886-gates (Claude): private copy; item removal ports bound (RET checked).
extern "C" {
unsigned __fastcall recovered_creature_remove_item_00510ef0(void*,void*,void*,unsigned,unsigned,void*); // 00510EF0 CSWSCreature::RemoveItem RET 10h (w2_party_native)
unsigned __fastcall recovered_placeable_remove_item_00584ac0(void*,void*,void*);                        // 00584AC0 CSWSPlaceable::RemoveItem RET 4 (w2_party_native)
unsigned __fastcall recovered_remove_item_00555fd0(void*,void*,void*);                                  // 00555FD0 CItemRepository::RemoveItem RET 4 (w2_party_native)
}
extern "C" void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void*,void*,int);
extern "C" void __fastcall recovered_item_remove_area_005538a0(void*,void*);
extern "C" unsigned __fastcall recovered_inventory_insert_0055d330(void*,void*,void**,unsigned,unsigned,unsigned);
extern "C" void __fastcall recovered_item_owner_00553210(void*,void*,unsigned,unsigned,unsigned,unsigned);
struct NativePlaceableInventory final:PlaceableInventoryPorts{
 NativeMemory&m;explicit NativePlaceableInventory(NativeMemory&v):m(v){}
 Word objects(Word a)override{return m.read(m.read(a+4)+0x1005c);}Word lookup(Word a,Word b,Word c)override{return kotor_fastcall_004d8230(ptr(a),nullptr,b,static_cast<void**>(ptr(c)));}Word byte(Word a)override{return m.read8(a);}
 Word cast(Word a,Word offset){using F=void*(__fastcall*)(void*,void*);return word(reinterpret_cast<F>(m.read(m.read(a)+offset))(ptr(a),nullptr));}Word as_creature(Word a)override{return cast(a,0x30);}Word as_placeable(Word a)override{return cast(a,0x48);}Word as_item(Word a)override{return cast(a,0x38);}
 void remove_creature(Word a,Word b,Word c,Word d,Word e)override{(void)recovered_creature_remove_item_00510ef0(ptr(a),nullptr,ptr(b),c,d,ptr(e));}void remove_placeable(Word a,Word b)override{(void)recovered_placeable_remove_item_00584ac0(ptr(a),nullptr,ptr(b));}Word base_item(Word a,Word b)override{return word(kotor_m15b_base_item_array_get_base_item_005b31a0(ptr(a),nullptr,static_cast<int>(b)));}void remove_inventory(Word a,Word b)override{(void)recovered_remove_item_00555fd0(ptr(a),nullptr,ptr(b));}void clear_item(Word a)override{recovered_item_remove_area_005538a0(ptr(a),nullptr);}void insert(Word a,Word b,Word c,Word d,Word e)override{(void)recovered_inventory_insert_0055d330(ptr(a),nullptr,static_cast<void**>(ptr(b)),c,d,e);}void owner(Word a,Word b,Word c,Word d,Word e)override{recovered_item_owner_00553210(ptr(a),nullptr,b,c,d,e);}
};
extern "C" unsigned __fastcall recovered_placeable_acquire_item_00584b10(void* self,void*,void* item_cell,unsigned container,unsigned flag); // 1126: W6 native 00584B10 supersedes 1154's portable (ruling); 1154 retired
Word native_placeable_inventory(Word a,Word b,Word c,Word d){return recovered_placeable_acquire_item_00584b10(ptr(a),nullptr,ptr(b),c,d);}
