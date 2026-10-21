#include "97f9e6186903abdf_layout_parser.hpp"
#include <cstring>
#include <new>
extern "C" {
void* __fastcall async_ref_construct_string_00406d60(void*,void*,void*);
void* __fastcall unresolved_ref_assign_00406290(void*,void*,void*,const char*);
void* __fastcall c2da_default_ref_00405ed0(void*,void*);
void* __fastcall GuiAssign005e5140(void*,void*,const char*);
void* __fastcall c2da_current_resource_demand_00409b10(void*,void*);
void __fastcall recovered_layout_reference_005de5f0(void*,void*,void*,unsigned);
int __cdecl kotor_sscanf_006fae08(const char*,const char*,...);
}
namespace {using namespace recovered_layout;void* ptr(U a){return reinterpret_cast<void*>(a);}char* chars(U a){return static_cast<char*>(ptr(a));}
struct Native final:Ports{
 U read(U a)override{U v;std::memcpy(&v,ptr(a),4);return v;}void write(U a,U v)override{std::memcpy(ptr(a),&v,4);}
 void ref_from_string(U a,U b)override{async_ref_construct_string_00406d60(ptr(a),nullptr,ptr(b));}
 void set_reference(U a,U b,U c)override{recovered_layout_reference_005de5f0(ptr(a),nullptr,ptr(b),c);}
 U demand(U a)override{return reinterpret_cast<U>(c2da_current_resource_demand_00409b10(ptr(a),nullptr));}
 U allocate(U n)override{return reinterpret_cast<U>(::operator new(n));}void zero(U a,U n)override{std::memset(ptr(a),0,n);}void copy(U a,U b,U n)override{std::memcpy(ptr(a),ptr(b),n);}void free(U a)override{::operator delete(ptr(a));}
 void scan_line(U a,U b)override{(void)kotor_sscanf_006fae08(chars(a),"%[^\r\n]",chars(b));}U length(U a)override{return static_cast<U>(std::strlen(chars(a)));}bool literal_equal(U a,const char*b,U n)override{return std::memcmp(ptr(a),b,n)==0;}
 void scan_count(U a,U b)override{(void)kotor_sscanf_006fae08(chars(a),"%*s%d",static_cast<int*>(ptr(b)));}
 void ref_empty(U a)override{c2da_default_ref_00405ed0(ptr(a),nullptr);}void vector_empty(U a)override{write(a,0);write(a+4,0);write(a+8,0);}void string_empty(U a)override{write(a,0);write(a+4,0);}
 void scan_room(U a,U b,U c)override{(void)kotor_sscanf_006fae08(chars(a),"%s%f%f%f",chars(b),static_cast<float*>(ptr(c)),static_cast<float*>(ptr(c+4)),static_cast<float*>(ptr(c+8)));}
 void ref_assign(U a,U b,U c)override{unresolved_ref_assign_00406290(ptr(a),nullptr,ptr(b),chars(c));}
 void scan_door(U a,U b,U c,U d,U e,U f)override{(void)kotor_sscanf_006fae08(chars(a),"%s%s%d%f%f%f%f%f%f%f",chars(b),chars(c),static_cast<int*>(ptr(d)),static_cast<float*>(ptr(e)),static_cast<float*>(ptr(e+4)),static_cast<float*>(ptr(e+8)),static_cast<float*>(ptr(f)),static_cast<float*>(ptr(f+4)),static_cast<float*>(ptr(f+8)),static_cast<float*>(ptr(f+12)));}
 void string_literal(U a,U b)override{GuiAssign005e5140(ptr(a),nullptr,chars(b));}
};}
extern "C" unsigned __fastcall recovered_layout_parse_005de900(void*self,void*,void*name){Native p;alignas(8) unsigned char scratch[0x80];return recovered_layout::parse(reinterpret_cast<unsigned>(self),reinterpret_cast<unsigned>(name),reinterpret_cast<unsigned>(scratch),p);}
