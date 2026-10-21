#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
// Exact x87 operation order for finite map coordinates; false leaves exceptional/out-of-range inputs unadmitted.
void area_fog_store(Word input_bits,Word* destination);
bool area_map_coordinate(Word input_bits,Word factor_bits,Word& output);
}
