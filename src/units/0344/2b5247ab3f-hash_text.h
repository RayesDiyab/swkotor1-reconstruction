#pragma once
#include "1c45e10a48-providers.h"
#include "e3a8e0daac-resources.h"
#include "44d8281443-lowercase.h"

// Declaration only. Integrator must publish the sole genuine key CRC cell,
// initialized by its original producer. This is separate from 007A4178.
extern "C" volatile uint32_t kotor_key_crc_table_007a3d78[256];
