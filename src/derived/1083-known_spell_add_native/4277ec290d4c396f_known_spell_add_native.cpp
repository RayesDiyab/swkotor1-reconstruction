#include "866872626c054eaf_known_spell_add.hpp"
#include <cstring>
namespace kotor_reconstruction{struct ArrayMutations;}
namespace menu_list_growth{void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*,void*,unsigned);}
namespace{using U=unsigned;void*ptr(U a){return reinterpret_cast<void*>(a);}struct Native:recovered_known_spell_add::Ports{U read(U a)override{U v;std::memcpy(&v,ptr(a),4);return v;}void write(U a,U b)override{std::memcpy(ptr(a),&b,4);}void grow(U a,U b)override{menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(ptr(a)),nullptr,b);}};}
extern "C" void __fastcall recovered_known_spell_add_00649660(void*self,void*,unsigned selector,unsigned spell){Native p;recovered_known_spell_add::add(reinterpret_cast<U>(self),selector,spell,p);}
