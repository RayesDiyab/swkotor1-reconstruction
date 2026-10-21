// closeB (Claude): private shadow copy (st/source/door_mesh_native_callbacks.inc; slot 10 -> 005CE160).
extern "C" unsigned __stdcall c2da_resource_constant_word_00406490();
Word __fastcall door_mesh_one(void*,void*){return 1;}
#define DOOR_MESH_GATE(name,address) Word __fastcall name(void*,void*){required(address);}
DOOR_MESH_GATE(door_mesh_slot1,0x5ce960)
#undef DOOR_MESH_GATE
Word __fastcall door_mesh_constant(void*,void*){return c2da_resource_constant_word_00406490();}
#define DMC(x) reinterpret_cast<const void*>(&x)
const void* const door_mesh_table_0074c3a0[]={DMC(door_mesh_one),DMC(door_mesh_slot1),DMC(mesh_base_slot2),DMC(door_mesh_reference_callback),DMC(mesh_reference),DMC(door_mesh_constant),DMC(door_mesh_constant),DMC(door_mesh_constant),DMC(door_mesh_constant),DMC(door_mesh_binary_callback),reinterpret_cast<const void*>(&kotor_fastcall_005ce160)/* closeB: 005CE160 */};
#undef DMC
struct NativeDoorMesh final:recovered_door_mesh::Ports {NativeMemory&m;explicit NativeDoorMesh(NativeMemory&v):m(v){}void base(Word a,Word b,Word c)override{NativeRoomWalkmesh p(m);(void)room_mesh_base_00596b80(m,p,a,b,c);}Word table()override{return reinterpret_cast<Word>(door_mesh_table_0074c3a0);}void write(Word a,Word b)override{m.write(a,b);}void half(Word a,Word b)override{m.write16(a,b);}void byte(Word a,Word b)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(b);}Word constant(Word a)override{switch(a){case 0x74c3cc:return 0x736f6c63;case 0x74c3d0:return 0x6465;case 0x74c3d2:return 0;case 0x74c3d4:case 0x74c3dc:return 0x6e65706f;case 0x74c3d8:return 0x31;case 0x74c3e0:return 0x32;default:required(a);}}};
[[maybe_unused]] Word native_door_mesh_construct(Word self){NativeMemory m;NativeDoorMesh p(m);return recovered_door_mesh::construct(self,p);}
[[maybe_unused]] void native_door_mesh_state(Word self,Word value){NativeMemory m;NativeDoorMesh p(m);recovered_door_mesh::state(self,value,p);}
