#pragma once
#include <cstdint>
namespace recovered_save_chargen {
struct String{std::uint32_t data,length;};struct Ref{char bytes[16];};struct Struct{std::uint32_t index;};struct List{std::uint32_t words[5];};struct Localized{std::uint32_t words[2];};
struct Ports {
 virtual ~Ports()=default;
 virtual void*pointer(void*,unsigned)=0;virtual void*rules()=0;virtual void*allocate(unsigned)=0;virtual void free(void*)=0;
 virtual void localized_construct(Localized*)=0;virtual void localized_add(Localized*,unsigned,void*,unsigned)=0;virtual void localized_destroy(Localized*)=0;
 virtual void* gff_construct(void*)=0;virtual void gff_initialize(void*,Struct*,String*,String*)=0;virtual void gff_destroy(void*,unsigned)=0;
 virtual void write_scalar(void*,Struct*,unsigned,const char*,unsigned)=0;virtual void write_string(void*,Struct*,void*,const char*)=0;virtual void write_ref(void*,Struct*,Ref*,const char*)=0;virtual void write_localized(void*,Struct*,Localized*,const char*)=0;
 virtual void add_list(void*,List*,Struct*,const char*)=0;virtual void add_struct(void*,Struct*,List*,unsigned)=0;virtual void write_file(void*,String*,unsigned)=0;
 virtual void string_default(String*)=0;virtual void string_literal(String*,const char*)=0;virtual void string_assign(String*,const char*)=0;virtual void string_copy(String*,String*)=0;virtual void string_destroy(String*)=0;virtual const char*text(String*)=0;virtual void string_concat(String*,String*,String*)=0;virtual void string_format_index(String*,const char*,const char*,unsigned)=0;virtual void ref_from_string(Ref*,String*)=0;
 virtual unsigned armor(void*,unsigned)=0;virtual unsigned class_level(void*,unsigned)=0;virtual unsigned class_id(void*,unsigned)=0;virtual unsigned spell_count(void*,unsigned,unsigned)=0;virtual unsigned spell(void*,unsigned,unsigned,unsigned)=0;virtual unsigned feat(void*,unsigned)=0;virtual unsigned max_hp(void*)=0;virtual Ref*portrait(void*,Ref*,unsigned)=0;
};
void save_006123e0(void*,Ports&);
}
