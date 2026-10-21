#include "b5a7895813-mesh_clip_numeric_native.inc"
extern "C" unsigned __fastcall recovered_mesh_crossings_00582630(void*,void*,void*,void*,unsigned*);
struct NativeMeshSegmentClip final:MeshSegmentClipPorts{
 bool equal(Word a,Word b)override{return aurora_vector_equal_004aa980(ptr(a),ptr(b))!=0;}
 Word plus_one(Word bits)override{float in,out,one=1.0f;std::memcpy(&in,&bits,4);__asm{fld in
 fadd one
 fstp out}Word result;std::memcpy(&result,&out,4);return result;}
 Word crossings(Word a,Word b,Word c,Word d)override{return recovered_mesh_crossings_00582630(ptr(a),nullptr,ptr(b),ptr(c),static_cast<unsigned*>(ptr(d)));}
 void length(Word s)override{mesh_clip_length(s);}
 bool first_compare(Word s,Word p,bool even)override{return mesh_clip_first_compare(s,p,even);}
 bool second_compare(Word s)override{return mesh_clip_second_compare(s);}
};
