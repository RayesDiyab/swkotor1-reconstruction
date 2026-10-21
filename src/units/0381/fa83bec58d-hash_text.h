#pragma once
#include "87fb61cae1-providers.h"
#include "868f97a4f5-resources.h"
#include "e2a1c02a6b-lowercase.h"

// Declaration only. Integrator must publish the sole genuine key CRC cell,
// initialized by its original producer. This is separate from 007A4178.
extern "C" volatile uint32_t kotor_key_crc_table_007a3d78[256];
