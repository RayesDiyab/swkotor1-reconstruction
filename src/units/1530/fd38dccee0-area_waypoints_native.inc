// 0886-gates (Claude): private copy; CSWSRoom::ComputeHeight 00579430 (Vector by value, int) RET 10h, ST0 result.
// (w5_room_compute_height_00579430(void*,void*,w5::Vector,w5::I) is declared by safe_position_search.inc; 1206-00 w5t3 walkmesh)
struct NativeAreaHeight final:AreaHeightPorts{
 NativeMemory&m;explicit NativeAreaHeight(NativeMemory&v):m(v){}
 Word find(Word a,Word b,Word c)override{NativeAreaFindRoom p;alignas(8)unsigned char scratch[0x48];return area_find_room_004bb600(m,p,a,b,c,word(scratch));}
 Word ground(Word a,Word b,Word x,Word y,Word z)override{NativeRoomHeight p;Word scratch[3];return room_height_bits_00579470(m,p,a,b,x,y,z,word(scratch));}
 Word alternate(Word a,Word b,Word c,Word d,Word e)override{w5::Vector p;std::memcpy(&p.x,&b,4);std::memcpy(&p.y,&c,4);std::memcpy(&p.z,&d,4);const float h=static_cast<float>(w5_room_compute_height_00579430(ptr(a),nullptr,p,static_cast<int>(e)));Word bits;std::memcpy(&bits,&h,4);return bits;}
};
struct NativeWaypointLoad final:WaypointLoadPorts{
 NativeMemory&m;NativeTriggerFields base;explicit NativeWaypointLoad(NativeMemory&v):m(v),base(v){}
 Word localized_construct(Word a)override{return base.localized_construct(a);}void localized_destroy(Word a)override{base.localized_destroy(a);}
 Word localized_read(Word a,Word b,Word c,Word d,Word e,Word f)override{if(d!=0x74c060)return base.localized_read(a,b,c,d,e,f);NativeLocalized p(m);unsigned scratch[6];return gff_localized_00411fd0(m,p,a,b,c,word(const_cast<char*>("MapNote")),e,f,word(scratch));}
 void localized_assign(Word a,Word b)override{base.localized_assign(a,b);}void string_empty(Word a)override{base.string_construct(a,0x73d71c);}Word string_read(Word a,Word b,Word c,Word d,Word e,Word f)override{return base.string_read(a,b,c,d,e,f);}void string_destroy(Word a)override{base.string_destroy(a);}void tag(Word a,Word b)override{base.tag(a,b);}
 Word floating(Word a,Word b,Word c,Word d,Word e)override{return base.floating(a,b,c,d,e);}void normalize(Word a)override{base.normalize_if_needed(a);}void position(Word a,Word b)override{base.position(a,b);}void orientation(Word a,Word b)override{base.orientation(a,b);}
 Word byte(Word a,Word b,Word c,Word d,Word e)override{const char*label;if(c==0x74c078)label="HasMapNote";else if(c==0x74c068)label="MapNoteEnabled";else required(c);return gui_load_gff_byte_00411a60(ptr(a),nullptr,ptr(b),label,static_cast<unsigned*>(ptr(d)),static_cast<unsigned char>(e));}
 Word module(Word a)override{return base.module(a);}void register_tag(Word a,Word b,Word c)override{base.register_tag(a,b,c);}
};
struct NativeAreaWaypoints final:AreaWaypointPorts{
 NativeMemory&m;explicit NativeAreaWaypoints(NativeMemory&v):m(v){}
 Word list(Word a,Word b,Word c,Word d)override{if(d!=0x7474d8)required(d);return gff_list_004118c0(ptr(a),nullptr,ptr(b),ptr(c),"WaypointList");}Word count(Word a,Word b)override{return gff_list_count_00411940(ptr(a),nullptr,ptr(b));}Word element(Word a,Word b,Word c,Word d)override{return gff_list_element_00411990(ptr(a),nullptr,ptr(b),ptr(c),d);}Word type(Word a,Word b)override{return static_cast<kotor_q05::CResGFF*>(ptr(a))->GetElementType(static_cast<kotor_q05::StructHandle*>(ptr(b)));}
 Word dword(Word a,Word b,Word c,Word d,Word e)override{if(c!=0x744c24)required(c);return gui_load_gff_dword_00411c20(ptr(a),nullptr,ptr(b),"ObjectId",static_cast<unsigned*>(ptr(d)),e);}Word allocate(Word a)override{return word(::operator new(a));}Word construct(Word a,Word b)override{return word(recovered_waypoint_construct_005c7e70(ptr(a),nullptr,b));}
 Word load(Word a,Word b,Word c)override{NativeWaypointLoad p(m);alignas(8)unsigned char scratch[0x58];return waypoint_load_005c7f30(m,p,a,b,c,word(scratch));}void destroy(Word a)override{using F=void*(__thiscall*)(void*,unsigned);(void)reinterpret_cast<F>(m.read(m.read(a)))(ptr(a),1);}void common(Word a,Word b,Word c)override{native_object_common(a,b,c);}
 Word floating(Word a,Word b,Word c,Word d,Word e)override{NativeTriggerFields p(m);return p.floating(a,b,c,d,e);}void place(Word a,Word b,Word c,Word d,Word e,Word f)override{recovered_waypoint_place_005c8580(ptr(a),nullptr,b,c,d,e,f);}
 Word height(Word a,Word b,Word c,Word d)override{NativeAreaHeight p(m);unsigned scratch[4];return area_height_bits_004bc380(m,p,a,b,c,d,word(scratch));}void position(Word a,Word b,Word c,Word d,Word e)override{native_object_position(a,b,c,d,e);}
};
