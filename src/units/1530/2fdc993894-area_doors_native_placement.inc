// 0886-gates (Claude): private copy; door destroy (0050A1DD..0050A1E3 vtbl[0](1)), LoadObjectState, list Add bound.
extern "C" void __fastcall w2x_object_state_004d1cf0(void*,void*,void*,void*);                 // 004D1CF0 RET 8 (this unit)
extern "C" void __fastcall recovered_door_placement_0058b6d0(void*,void*,unsigned,unsigned,unsigned,unsigned,unsigned);
extern "C" void* __fastcall recovered_door_construct_00589ee0(void*,void*,unsigned);
struct NativeAreaDoors final:AreaDoorPorts{
 Word list(Word a,Word b,Word c)override{return gff_list_004118c0(ptr(a),nullptr,ptr(b),ptr(c),"Door List");}
 Word count(Word a,Word b)override{return gff_list_count_00411940(ptr(a),nullptr,ptr(b));}
 Word element(Word a,Word b,Word c,Word d)override{return gff_list_element_00411990(ptr(a),nullptr,ptr(b),ptr(c),d);}
 Word type(Word a,Word b)override{return static_cast<kotor_q05::CResGFF*>(ptr(a))->GetElementType(static_cast<kotor_q05::StructHandle*>(ptr(b)));}
 Word dword(Word a,Word b,Word c)override{return gui_load_gff_dword_00411c20(ptr(a),nullptr,ptr(b),"ObjectId",static_cast<unsigned*>(ptr(c)),0x7f000000);}
 Word allocate(Word a)override{return word(::operator new(a));}
 Word construct(Word a,Word b)override{return word(recovered_door_construct_00589ee0(ptr(a),nullptr,b));}
 Word load(Word a,Word b,Word c,Word d)override{NativeMemory m;NativeDoorLoad p(m);unsigned char scratch[0x80];return door_load_0058c5f0(m,p,a,b,c,d,word(scratch));}void destroy(Word a)override{NativeMemory m;using F=void*(__thiscall*)(void*,Word);(void)reinterpret_cast<F>(m.read(m.read(a)))(ptr(a),1);}Word floating(Word a,Word b,Word c,Word d)override{const char*label=nullptr;switch(c){case 0x747a8c:label="Bearing";break;case 0x747a88:label="Z";break;case 0x74699c:label="Y";break;case 0x7476e8:label="X";break;default:required(c);}float v=gui_load_gff_float_00411d00(ptr(a),nullptr,ptr(b),label,static_cast<unsigned*>(ptr(d)),0.0f);Word bits;std::memcpy(&bits,&v,4);return bits;}void axis_angle(Word a,Word b,Word c,Word d,Word e)override{kotor_scene3d::Vector v;float angle;std::memcpy(&v.x,&b,4);std::memcpy(&v.y,&c,4);std::memcpy(&v.z,&d,4);std::memcpy(&angle,&e,4);(void)quaternion_construct_004ac960(static_cast<kotor_scene3d::Quaternion*>(ptr(a)),nullptr,v,angle);}void rotation(Word a,Word b,Word c,Word d,Word e)override{recovered_door_rotation_005896e0(ptr(a),nullptr,b,c,d,e);}void common(Word a,Word b,Word c)override{w2x_object_state_004d1cf0(ptr(a),nullptr,ptr(b),ptr(c));}void place(Word a,Word b,Word c,Word d,Word e,Word f)override{recovered_door_placement_0058b6d0(ptr(a),nullptr,b,c,d,e,f);}void append(Word a,Word b)override{gui_load_array_add_00671c00(static_cast<kotor_reconstruction::ArrayMutations*>(ptr(a)),nullptr,b);}
};
