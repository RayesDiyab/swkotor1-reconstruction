// 0886-gates (Claude): the former required(0x...) party ports bound to real providers / claude_0886_bodies_party.cpp.
extern "C" {
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void*,void*);                     // 0060FB20 CSWCCreature::GetServerCreature (0816)
unsigned __fastcall recovered_area_next_object_005071e0(void*,void*,unsigned*);                    // 005071E0 CSWSArea::GetNextObjectInArea (w2_update_native)
unsigned __fastcall recovered_object_get_reputation_0057cb80(void*,void*,unsigned,unsigned);        // 0057CB80 CSWSObject::GetReputation (w1_native_b7)
unsigned __fastcall recovered_compute_safe_location_004aeb60(void*,void*,void*,float,unsigned,unsigned,void*); // 004AEB60 ComputeSafeLocation RET 14h (w2_party_native)
void __fastcall recovered_object_position_004cd1a0(void*,void*,void*,unsigned,unsigned,unsigned);   // 004CD1A0 SetPosition RET 10h (this unit)
void* __fastcall recovered_effect_construct_00503e40(void*,void*,unsigned);                         // 00503E40 CGameEffect::CGameEffect RET 4 (this unit)
void __fastcall recovered_effect_apply_004d14f0(void*,void*,void*,unsigned,unsigned);               // 004D14F0 CSWSObject::ApplyEffect RET 0Ch (this unit)
unsigned __fastcall recovered_creature_incapacitated_004ef890(void*,void*);                       // 004EF890 GetDeadTemp (0963)
unsigned __fastcall kotor_fastcall_004f7650(void*,void*);                                          // 004F7650 (claude_0886_bodies_party)
struct Claude0886Vec{float x,y,z;};
void __fastcall kotor_fastcall_005790c0(void*,void*,int,Claude0886Vec);                             // 005790C0 SetPartyMemberWorldLocation RET 10h
void __fastcall kotor_fastcall_00579130(void*,void*,int,Claude0886Vec);                             // 00579130 SetPartyMemberWorldOrientation RET 10h
void __fastcall kotor_fastcall_005792d0(void*,void*,Claude0886Vec,int);                             // 005792D0 SetWorldPointExplored RET 10h
}
inline Claude0886Vec claude_0886_vec(VectorWords v){Claude0886Vec r;std::memcpy(&r.x,&v.x,4);std::memcpy(&r.y,&v.y,4);std::memcpy(&r.z,&v.z,4);return r;}
extern "C" unsigned __fastcall recovered_area_first_object_005071b0(void*,void*,unsigned*); // 1127 W2 optional
// Typed adapters for reviewed party callers. Missing providers fail with their exact address.
struct NativePartyPosition:PartyPositionPorts {NativeMemory& m;explicit NativePartyPosition(NativeMemory& a):m(a){}
 Word party_005ed8b0(Word a) override{return word(kotor_fastcall_005ed8b0(ptr(a),nullptr));}
 Word module_004b14f0(Word a) override{return word(server_internal_get_module_004b14f0(ptr(a),nullptr));}
 Word character_006346c0(Word a,Word i) override{return word(frame_GetCharacter_006346c0(ptr(a),nullptr,i));}
 void reveal_005792d0(Word a,VectorWords v,Word r) override{kotor_fastcall_005792d0(ptr(a),nullptr,claude_0886_vec(v),static_cast<int>(r));}
 void map_position_005790c0(Word a,Word i,VectorWords v) override{kotor_fastcall_005790c0(ptr(a),nullptr,static_cast<int>(i),claude_0886_vec(v));}
 void map_orientation_00579130(Word a,Word i,VectorWords v) override{kotor_fastcall_00579130(ptr(a),nullptr,static_cast<int>(i),claude_0886_vec(v));}
};
struct NativePartyUpdate:PartyUpdatePorts {NativeMemory& m;explicit NativePartyUpdate(NativeMemory& a):m(a){}
 Word cinematic_005edc60(Word a0) override {return m.read(m.read(a0+4)+0x2c0);}
 Word busy_005edad0(Word a0) override {return m.read(m.read(a0+4)+0x288);}
 Wide high_time_005e9260(Word a0) override {return static_cast<Wide>(static_cast<menu_continuation::ExoTimers*>(ptr(a0))->GetHighResolutionTimer_005e9260());}
 Wide divide_006fb030(Wide a0,Wide a1) override {return a0/a1;}
 Word party_005ed8b0(Word a0) override {return word(kotor_fastcall_005ed8b0(ptr(a0),nullptr));}
 Word character_006346c0(Word a0,Word a1) override {return word(frame_GetCharacter_006346c0(ptr(a0),nullptr,a1));}
 Word creature_0060fb20(Word a0) override {return word(kotor_m15a_creature_get_server_creature_0060fb20(ptr(a0),nullptr));}
 Word incapacitated_004ef890(Word a0) override {return recovered_creature_incapacitated_004ef890(ptr(a0),nullptr);}
 void all_down_005edc40(Word a) override {kotor_fastcall_005edc40(ptr(a),nullptr);/* closeB: claude_closeB_death.cpp */}
 Word area_004cb120(Word a0) override {return word(recovered_object_area_004cb120(ptr(a0),nullptr));}
 Word object_array_004aed70(Word a0) override {return m.read(m.read(a0+4)+0x1005c);}
 Word first_005071b0(Word a,Word b) override {return recovered_area_first_object_005071b0(ptr(a),nullptr,static_cast<unsigned*>(ptr(b)));}
 Word next_005071e0(Word a0,Word a1) override {return recovered_area_next_object_005071e0(ptr(a0),nullptr,static_cast<unsigned*>(ptr(a1)));}
 Word lookup_004d8230(Word a0,Word a1,Word a2) override {return kotor_fastcall_004d8230(ptr(a0),nullptr,a1,static_cast<void**>(ptr(a2)));}
 Word virtual_creature_30(Word a0) override {using F=void*(__thiscall*)(void*);return word(reinterpret_cast<F>(m.read(m.read(a0)+0x30))(ptr(a0)));}
 Word relationship_0057cb80(Word a0,Word a1,Word a2) override {return recovered_object_get_reputation_0057cb80(ptr(a0),nullptr,a1,a2);}
 Word hostile_004f7650(Word a0) override {return kotor_fastcall_004f7650(ptr(a0),nullptr);}
 Word safe_position_004aeb60(Word a0,Word a1,Word a2,Word a3,Word a4,Word a5) override {float radius;std::memcpy(&radius,&a2,4);return recovered_compute_safe_location_004aeb60(ptr(a0),nullptr,ptr(a1),radius,a3,a4,ptr(a5));}
 void set_position_004cd1a0(Word a0,Word a1,Word a2,Word a3,Word a4) override {recovered_object_position_004cd1a0(ptr(a0),nullptr,ptr(a1),a2,a3,a4);}
 Word allocate_006fa7e6(Word a0) override {return word(::operator new(a0));}
 Word effect_ctor_00503e40(Word a0,Word a1) override {return word(recovered_effect_construct_00503e40(ptr(a0),nullptr,a1));}
 void apply_effect_004d14f0(Word a0,Word a1,Word a2,Word a3) override {recovered_effect_apply_004d14f0(ptr(a0),nullptr,ptr(a1),a2,a3);}
 Word party_table_004aee70(Word a0) override {return word(lane_config_required::GetPartyTable_004aee70(ptr(a0),nullptr));}
 bool distance_exceeds_004b7120(Word a0,Word a1) override {float p[3],q[3];std::memcpy(p,ptr(a0),12);std::memcpy(q,ptr(a1),12);const double dx=static_cast<double>(p[0])-q[0],dy=static_cast<double>(p[1])-q[1],dz=static_cast<double>(p[2])-q[2];return (dz*dz+dy*dy)+dx*dx>1600.0;} // 004B7120..004B715F (fcomp [00745548]; test ah,41h)
};
