// One canonical source storage definition per original data identity.
// Original cold state is zero; genuine selected constructors initialize it.
// No precomputed table, alternate initializer, ready flag or absolute mapping.
#include "43a96bd70c-gff_hash_init.h"
#include "2b5247ab3f-hash_text.h"
extern "C" {
volatile uint32_t kotor_key_crc_table_007a3d78[256];
uint32_t kotor_gff_crc_table_007a4178[256];
}
static_assert(sizeof(kotor_key_crc_table_007a3d78)==1024 &&
              sizeof(kotor_gff_crc_table_007a4178)==1024,
              "Distinct original256-DWORD cold storage identities");
