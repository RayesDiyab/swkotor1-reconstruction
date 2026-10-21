#pragma once
#include "39846eee35-providers.h"
#include "7d923e84a8-resources.h"
#include "5e560aa399-lowercase.h"

// Declaration only. Integrator must publish the sole genuine key CRC cell,
// initialized by its original producer. This is separate from 007A4178.
extern "C" volatile uint32_t kotor_key_crc_table_007a3d78[256];
