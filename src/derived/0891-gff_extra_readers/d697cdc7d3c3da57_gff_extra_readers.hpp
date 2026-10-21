#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct GffExtraPorts{virtual ~GffExtraPorts()=default;virtual Word label_00411630(Word,Word,Word)=0;virtual Word field_00410990(Word,Word,Word)=0;virtual Word data_00410a20(Word,Word,Word)=0;virtual void write_byte(Word,Word)=0;};
// WORD result claims AX only; upper original EAX is unspecified.
Word gff_short_00411b40(Memory&,GffExtraPorts&,Word,Word,Word,Word,Word);
Wide gff_wide_00411d70(Memory&,GffExtraPorts&,Word,Word,Word,Word,Wide,Word);
Word gff_data_00412380(Memory&,GffExtraPorts&,Word,Word,Word,Word,Word,Word,Word,Word);
}
