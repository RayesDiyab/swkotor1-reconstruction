// 0886-gates (Claude): private copy; every NativeAreaItems port bound to its real provider (checked against 00504DE0:
// 00504E6E ReadFieldDWORD(element,"ObjectId",&status,7F000000h); 00504E89 new 29Ch; 00504EA8 CSWSItem(id);
// 00504ECE CResRef(""); 00504EF7 ReadFieldCResRef(out,element,"TemplateResRef",&status,&default); 00504F29 vtbl[0](1);
// 00504F75.. ReadFieldFLOAT(element,label,&status,0.0f); 00504FC3 AddToArea(area,x,y,z,!saved); 00505024 SetOrientation).
extern "C" {
void* __fastcall recovered_item_construct_005530a0(void*,void*,unsigned);                      // 005530A0 CSWSItem::CSWSItem RET 4 (0963)
unsigned __fastcall recovered_item_template_005608b0(void*,void*,void*);                       // 005608B0 CSWSItem::LoadFromTemplate RET 4 (0995)
unsigned __fastcall recovered_load_item_00560970(void*,void*,void*,void*);                     // 00560970 CSWSItem::LoadItem RET 8 (w2_party_native)
void __fastcall w2x_object_state_004d1cf0(void*,void*,void*,void*);                            // 004D1CF0 CSWSObject::LoadObjectState RET 8 (this unit)
void __fastcall recovered_item_add_to_area_00555720(void*,void*,void*,float,float,float,unsigned); // 00555720 CSWSItem::AddToArea RET 14h (w1_native_b7)
void __fastcall w1x_object_set_orientation_004cd170(void*,void*,void*);                       // 004CD170 CSWSObject::SetOrientation RET 4 (this unit)
}
#include "49d83a3625-area_items.hpp"
struct NativeAreaItems final:AreaItemPorts{
 Word list(Word a,Word b,Word c)override{return gff_list_004118c0(ptr(a),nullptr,ptr(b),ptr(c),"List");}
 Word count(Word a,Word b)override{return gff_list_count_00411940(ptr(a),nullptr,ptr(b));}
 Word element(Word a,Word b,Word c,Word d)override{return gff_list_element_00411990(ptr(a),nullptr,ptr(b),ptr(c),d);}
 Word type(Word a,Word b)override{return static_cast<kotor_q05::CResGFF*>(ptr(a))->GetElementType(static_cast<kotor_q05::StructHandle*>(ptr(b)));}
 Word dword(Word a,Word b,Word c)override{return gui_load_gff_dword_00411c20(ptr(a),nullptr,ptr(b),"ObjectId",static_cast<unsigned*>(ptr(c)),0x7f000000);}Word allocate(Word a)override{return word(::operator new(a));}Word construct(Word a,Word b)override{return word(recovered_item_construct_005530a0(ptr(a),nullptr,b));}void ref_empty(Word a)override{(void)static_cast<menu_resources::Ref*>(ptr(a))->construct_00406d80("");}void ref_read(Word a,Word b,Word c,Word d,Word e)override{(void)gui_load_gff_resref_00411e10(ptr(a),nullptr,ptr(b),ptr(c),"TemplateResRef",static_cast<unsigned*>(ptr(d)),ptr(e));}Word load_template(Word a,Word b)override{return recovered_item_template_005608b0(ptr(a),nullptr,ptr(b));}Word load_gff(Word a,Word b,Word c)override{return recovered_load_item_00560970(ptr(a),nullptr,ptr(b),ptr(c));}void destroy(Word a)override{NativeMemory m;using F=void*(__thiscall*)(void*,Word);(void)reinterpret_cast<F>(m.read(m.read(a)))(ptr(a),1);}void common(Word a,Word b,Word c)override{w2x_object_state_004d1cf0(ptr(a),nullptr,ptr(b),ptr(c));}Word floating(Word a,Word b,Word c,Word d)override{const float v=gui_load_gff_float_00411d00(ptr(a),nullptr,ptr(b),load_literal(c),static_cast<unsigned*>(ptr(d)),0.0f);Word bits;std::memcpy(&bits,&v,4);return bits;}void place(Word a,Word b,Word c,Word d,Word e,Word f)override{float x,y,z;std::memcpy(&x,&c,4);std::memcpy(&y,&d,4);std::memcpy(&z,&e,4);recovered_item_add_to_area_00555720(ptr(a),nullptr,ptr(b),x,y,z,f);}void orient(Word a,Word b)override{w1x_object_set_orientation_004cd170(ptr(a),nullptr,ptr(b));}
};
