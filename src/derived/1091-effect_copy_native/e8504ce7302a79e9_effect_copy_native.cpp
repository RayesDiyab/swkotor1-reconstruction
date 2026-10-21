#include "e6848e0e12af41d3_effect_copy.hpp"
#include <cstring>
#include <new>
extern "C"{void* __fastcall recovered_effect_construct_00503e40(void*,void*,unsigned);void* __fastcall async_assign_string_005e5c50(void*,void*,void*);}
namespace{using U=unsigned;void*ptr(U a){return reinterpret_cast<void*>(a);}U word(void*p){return reinterpret_cast<U>(p);}struct Native:recovered_effect_copy::Ports{U read(U a)override{U v;std::memcpy(&v,ptr(a),4);return v;}U half(U a)override{unsigned short v;std::memcpy(&v,ptr(a),2);return v;}void write(U a,U b)override{std::memcpy(ptr(a),&b,4);}void put_half(U a,U b)override{unsigned short v=static_cast<unsigned short>(b);std::memcpy(ptr(a),&v,2);}U allocate(U n)override{return word(::operator new(n));}void release(U a)override{::operator delete(ptr(a));}U construct(U a,U mode)override{return word(recovered_effect_construct_00503e40(ptr(a),nullptr,mode));}void zero(U a,U bytes)override{if(bytes)std::memset(ptr(a),0,bytes);}void string_assign(U a,U b)override{async_assign_string_005e5c50(ptr(a),nullptr,ptr(b));}};}
extern "C" void __fastcall recovered_effect_copy_00504090(void*self,void*,void*source,unsigned skip){Native p;recovered_effect_copy::copy(word(self),word(source),skip,p);}
extern "C" void __fastcall recovered_effect_resize_00503630(void*self,void*,unsigned count){Native p;recovered_effect_copy::resize(word(self),count,p);}
