// 0886-gates (Claude): private copy; area removal ports bound (RET 4 each).
extern "C" {
unsigned __fastcall kotor_fastcall_0058fec0(void*,void*,unsigned);   // 0058FEC0 CSWArea::RemoveStaticBoundingBox (claude_w1_route_bodies unit)
unsigned __fastcall kotor_fastcall_0050dc40(void*,void*,unsigned);   // 0050DC40 CSWSArea::RemoveObjectFromArea (claude_combat_spell_mine)
}
extern "C" unsigned recovered_placeable_debug_00832854=0;
extern "C" void* __fastcall recovered_object_area_004cb120(void*,void*);
extern "C" unsigned __fastcall recovered_area_bounds_append_0058fd50(void*,void*,unsigned,void*,void*);
extern "C" unsigned __fastcall recovered_area_insert_0050dfd0(void*,void*,unsigned,unsigned);
struct NativePlaceablePlacementMemory:NativeMemory{Word read(Word a)override{return a==0x832854?recovered_placeable_debug_00832854:NativeMemory::read(a);}};
struct NativePlaceablePlacement final:PlaceablePlacementPorts,recovered_creature_area_access::Ports{
 NativeMemory&m;NativeDoorFields base;explicit NativePlaceablePlacement(NativeMemory&v):m(v),base(v){}
 Word area(Word a)override{return word(recovered_object_area_004cb120(ptr(a),nullptr));}
 void remove_bounds(Word a,Word b)override{(void)kotor_fastcall_0058fec0(ptr(a),nullptr,b);}void remove_object(Word a,Word b)override{(void)kotor_fastcall_0050dc40(ptr(a),nullptr,b);}
 Word read(Word a)override{return m.read(a);}void write(Word a,Word b)override{m.write(a,b);}Word invalid()override{return 0x7f000000;}void set_area(Word a,Word b)override{recovered_creature_area_access::set(a,b,*this);}void position(Word a,Word b,Word c,Word d,Word e)override{native_object_position(a,b,c,d,e);}
 void string_zero(Word a)override{m.write(a,0);m.write(a+4,0);}void string_construct(Word a,Word b)override{const char*s=nullptr;switch(b){case 0x749820:s="PreciseUse";break;case 0x747f78:s="Hostile";break;case 0x749814:s="ModelName";break;default:required(b);}frame_LogStringCtor_005e5a90(ptr(a),nullptr,s);}void string_destroy(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}Word byte(Word a)override{return *static_cast<unsigned char*>(ptr(a));}
 void table_integer(Word a,Word b,Word c,Word d)override{(void)c2da_current_get_int_00413660(ptr(a),nullptr,b,ptr(c),static_cast<unsigned*>(ptr(d)));}void table_string(Word a,Word b,Word c,Word d)override{base.table_string(a,b,c,d);}void ref_from_string(Word a,Word b)override{(void)async_ref_construct_string_00406d60(ptr(a),nullptr,ptr(b));}void ref_assign(Word a,Word b,Word c)override{base.ref_assign(a,b,c);}
 Word mesh_load(Word a,Word b)override{NativeRoomMeshLoad p(m);alignas(4)unsigned char scratch[0x50];return room_mesh_load_00596670(m,p,a,b,word(scratch));}
 Word yaw_scaled(Word a)override{Word result;const Word factor=0x3c8efa35;__asm{push a}__asm{call kotor_m15a_yaw_004aa0f0}__asm{add esp,4}__asm{fmul dword ptr factor}__asm{fstp dword ptr result}return result;}
 Word axis_angle(Word a,Word b,Word c,Word d,Word e)override{kotor_scene3d::Vector v;float angle;std::memcpy(&v.x,&b,4);std::memcpy(&v.y,&c,4);std::memcpy(&v.z,&d,4);std::memcpy(&angle,&e,4);return word(quaternion_construct_004ac960(static_cast<kotor_scene3d::Quaternion*>(ptr(a)),nullptr,v,angle));}
 void bounds(Word a,Word b,Word c)override{NativeMeshVertexBounds p(m);alignas(4)unsigned char scratch[0x30];(void)mesh_vertex_bounds_00596d10(m,p,a,b,c,word(scratch));}void debug(Word,Word,Word,Word)override{required(0x599f70);}
 Word register_bounds(Word a,Word b,Word c,Word d)override{return recovered_area_bounds_append_0058fd50(ptr(a),nullptr,b,ptr(c),ptr(d));}void insert(Word a,Word b,Word c)override{(void)recovered_area_insert_0050dfd0(ptr(a),nullptr,b,c);}
};
void native_placeable_placement(Word a,Word b,Word c,Word d){NativePlaceablePlacementMemory m;NativePlaceablePlacement p(m);alignas(4)unsigned char scratch[0x90];placeable_placement_00587170(m,p,a,b,c,d,word(scratch));}
