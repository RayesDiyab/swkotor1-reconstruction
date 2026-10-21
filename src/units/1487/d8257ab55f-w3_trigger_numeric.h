#pragma once
#include "a43a781a46-w3_trigger_internal.h"
namespace trigger_numeric {
using namespace trigger_private;
struct Extended {unsigned char bytes[10];};
struct Direction {float x,z;Extended y;};
struct Center {float xyz[3];Extended z;};
std::uint16_t compare(const void*,const void*);
void float_copy(const void*,void*);
void negative_forward(const void*,Direction*);
void make_center(const U*,const U*,Center*);
std::uint8_t center_side(const Direction*,const Center*,const void*);
void negative_vector(const void*,void*);
std::uint8_t point_side(const void*,const void*,const void*);
void scale_coordinate(I,const void*,void*);
}
