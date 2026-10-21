#include "7f5c11fd6fcf3759_room_base.hpp"
#include <cstring>
#include <new>
extern "C" {void* __fastcall c2da_default_ref_00405ed0(void*,void*);int __fastcall c2da_equal_text_004060d0(void*,void*,const char*);void* __fastcall c2da_copy_00406120(void*,void*,void*,void*);void* __fastcall recovered_room_base_destroy_005c7d50(void*,void*,unsigned);}
namespace {using namespace recovered_room_base;void* ptr(U u){return reinterpret_cast<void*>(u);}void* table[]={reinterpret_cast<void*>(&recovered_room_base_destroy_005c7d50)};
struct Native final:Ports{U read(U a)override{U v;std::memcpy(&v,ptr(a),4);return v;}void write(U a,U v)override{std::memcpy(ptr(a),&v,4);}U table()override{return reinterpret_cast<U>(::table);}void empty_ref(U a)override{c2da_default_ref_00405ed0(ptr(a),nullptr);}U special_ref(U a)override{return static_cast<U>(c2da_equal_text_004060d0(ptr(a),nullptr,"****"));}void assign_ref(U a,U out,U b)override{c2da_copy_00406120(ptr(a),nullptr,ptr(out),ptr(b));}void release(U a)override{::operator delete(ptr(a));}};}
extern "C" void* __fastcall recovered_room_base_construct_005c7d10(void*self,void*){Native p;return ptr(recovered_room_base::construct(reinterpret_cast<unsigned>(self),p));}
extern "C" void __fastcall recovered_room_base_assign_005c7d70(void*self,void*,void*ref,void*position){Native p;alignas(4)unsigned char scratch[16];recovered_room_base::assign(reinterpret_cast<unsigned>(self),reinterpret_cast<unsigned>(ref),reinterpret_cast<unsigned>(position),reinterpret_cast<unsigned>(scratch),p);}
extern "C" void* __fastcall recovered_room_base_destroy_005c7d50(void*self,void*,unsigned flags){Native p;return ptr(recovered_room_base::destroy(reinterpret_cast<unsigned>(self),flags,p));}
extern "C" void __fastcall recovered_room_base_reset_005c7d00(void*self,void*){Native p;p.write(reinterpret_cast<unsigned>(self),p.table());}
