// closeB (Claude): private shadow copy (st/area-creatures/placeable_mesh_native.inc; slot 10 -> 005CDAD0).

#define PMC(x) reinterpret_cast<const void*>(&x)
const void* const placeable_mesh_table_0074c32c[]={PMC(door_mesh_one),PMC(mesh_base_slot1),PMC(mesh_base_slot2),PMC(mesh_reference),PMC(mesh_reference),PMC(door_mesh_constant),PMC(door_mesh_constant),PMC(door_mesh_constant),PMC(door_mesh_constant),PMC(mesh_base_slot9),reinterpret_cast<const void*>(&kotor_fastcall_005cdad0)/* closeB: 005CDAD0 */};
#undef PMC
struct NativePlaceableMesh final:recovered_placeable_mesh::Ports{NativeMemory&m;explicit NativePlaceableMesh(NativeMemory&v):m(v){}void base(Word a,Word b,Word c)override{NativeRoomWalkmesh p(m);(void)room_mesh_base_00596b80(m,p,a,b,c);}void write(Word a,Word b)override{m.write(a,b);}Word table()override{return reinterpret_cast<Word>(placeable_mesh_table_0074c32c);}};
extern "C" void* __fastcall recovered_placeable_mesh_construct_005cdaa0(void*self,void*){NativeMemory m;NativePlaceableMesh p(m);return ptr(recovered_placeable_mesh::construct(word(self),p));}
