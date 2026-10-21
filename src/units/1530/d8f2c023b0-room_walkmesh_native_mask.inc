// 0886-gates (Claude): private copy; walkmesh vtable slots / AABB list copy bound to real bodies (RET checked).
extern "C" {
unsigned __fastcall kotor_m15e_slot0d4_true_0063e7a0(void*,void*);       // 0063E7A0 return_true RET 0 (0832)
void __fastcall kotor_fastcall_00599ed0(void*,void*,void*);               // 00599ED0 TransformToWorld RET 4 (claude_0886_bodies_mesh)
void* __fastcall kotor_fastcall_00581bd0(void*,void*,const void*);        // 00581BD0 AABB list operator= RET 4 (claude_0886_bodies_mesh)
}
extern "C" int __fastcall c2da_current_get_int_00413660(void*,void*,unsigned,void*,unsigned*);
#define MESH_GATE(name,address) Word __fastcall name(void*,void*){required(address);}
Word __fastcall mesh_pure(void*a,void*b){return kotor_m15e_slot0d4_true_0063e7a0(a,b);} // 0063E7A0 return_true (0832)
MESH_GATE(mesh_base_slot1,0x59a040)
void __fastcall mesh_base_slot2(void*a,void*,void*b){kotor_fastcall_00599ed0(a,nullptr,b);} // 00599ED0 CSWCollisionMesh::TransformToWorld RET 4
Word __fastcall mesh_base_slot9(void*a,void*,void*b){NativeMemory m;return room_mesh_binary_base_00597120(m,word(a),word(b));}
Word __fastcall mesh_base_slot10(void*a,void*,Word,Word){NativeMemory m;m.write(word(a)+0x4c,0);return 1;} // 00596890..0059689C CSWCollisionMesh::LoadMeshText: [+4C]=0, EAX=1, RET 8
MESH_GATE(mesh_slot1,0x581370)
extern "C" void __fastcall recovered_mesh_position_00581af0(void*,void*,void*);
void __fastcall mesh_slot2(void*a,void*,void*b){recovered_mesh_position_00581af0(a,nullptr,b);}
Word __fastcall mesh_slot5(void*a,void*){NativeMemory m;return m.read(word(a)+0xe0);}
Word __fastcall mesh_slot6(void*a,void*){NativeMemory m;return m.read(word(a)+0xd8);} // 005821B0 GetLOSMaterialMask
Word __fastcall mesh_slot7(void*a,void*){NativeMemory m;return m.read(word(a)+0xdc);} // 005821C0 GetWalkableMaterialMask
Word __fastcall mesh_slot8(void*a,void*){NativeMemory m;return m.read(word(a)+0xe4);} // 005821D0 GetAllMaterialsMask
Word __fastcall mesh_slot9(void*a,void*,void*b){NativeMemory m;return room_mesh_binary_005807c0(m,word(a),word(b));}
#undef MESH_GATE
void __fastcall mesh_reference(void*a,void*,void*out){alignas(8) unsigned char scratch[16];(void)resource_ref_assignment::assign_00406120(static_cast<menu_resources::Ref*>(out),nullptr,reinterpret_cast<menu_resources::Ref*>(scratch),reinterpret_cast<const menu_resources::Ref*>(static_cast<unsigned char*>(a)+0xc));}
#define M(x) reinterpret_cast<void*>(&x)
const void* const mesh_base_vtable[]={M(mesh_pure),M(mesh_base_slot1),M(mesh_base_slot2),M(mesh_reference),M(mesh_reference),M(gff_constant),M(gff_constant),M(gff_constant),M(gff_constant),M(mesh_base_slot9),M(mesh_base_slot10)};
const void* const mesh_vtable[]={M(mesh_pure),M(mesh_slot1),M(mesh_slot2),M(mesh_reference),M(mesh_reference),M(mesh_slot5),M(mesh_slot6),M(mesh_slot7),M(mesh_slot8),M(mesh_slot9),reinterpret_cast<const void*>(&kotor_fastcall_00582d70)/* walkmesh: 00582D70 CSWRoomSurfaceMesh::LoadMeshText */};
#undef M
// 0886-gates: original mesh vtable address -> rebuilt table (claude_0886_bodies_mesh destructors).
extern "C" void* __cdecl claude_0886_mesh_table(unsigned original){if(original==0x74944c)return const_cast<void*>(static_cast<const void*>(mesh_vtable));if(original==0x74a068)return const_cast<void*>(static_cast<const void*>(mesh_base_vtable));required(original);}
struct NativeRoomWalkmesh:RoomWalkmeshPorts{NativeMemory&m;explicit NativeRoomWalkmesh(NativeMemory&v):m(v){}
 Word vtable(Word a)override{if(a==0x74a068)return reinterpret_cast<Word>(mesh_base_vtable);if(a==0x74944c)return reinterpret_cast<Word>(mesh_vtable);required(a);}void ref_default(Word a)override{(void)c2da_default_ref_00405ed0(ptr(a),nullptr);}
 void list_copy(Word a,Word b)override{(void)kotor_fastcall_00581bd0(ptr(a),nullptr,ptr(b));} // 00581BD0 CExoArrayList<AABBNode>::operator= RET 4
 void empty(Word a)override{m.write(a,0);m.write(a+4,0);}void literal(Word a,Word b)override{const char*t=nullptr;switch(b){case 0x749490:t="Label";break;case 0x749484:t="LineOfSight";break;case 0x747670:t="Walk";break;case 0x749478:t="WalkCheck";break;default:required(b);}frame_LogStringCtor_005e5a90(ptr(a),nullptr,t);}
 void destroy(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}void table_string(Word a,Word b,Word c,Word d)override{(void)c2da_current_get_string_00413270(ptr(a),nullptr,static_cast<int>(b),ptr(c),ptr(d));}void table_integer(Word a,Word b,Word c,Word d)override{(void)c2da_current_get_int_00413660(ptr(a),nullptr,b,ptr(c),static_cast<unsigned*>(ptr(d)));}
};
