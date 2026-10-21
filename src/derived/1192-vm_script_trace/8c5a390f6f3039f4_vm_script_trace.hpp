#pragma once
#include <cstdint>
namespace recovered_vm_script_trace {
using Word=std::uint32_t;
struct Ports {virtual ~Ports()=default;virtual Word read(Word)=0;virtual Word ai(Word)=0;virtual Word not_empty(Word)=0;virtual void comma(Word)=0;virtual Word concat(Word,Word,Word)=0;virtual void assign(Word,Word)=0;virtual void destroy(Word)=0;};
void append(Ports&,Word ai,Word script,Word scratch);
void command(Ports&,Word script,Word scratch);
}
