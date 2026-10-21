#include "9f442eedcf-file_thread.h"
namespace kotor_file {
FileThread global_file_thread{}; // original 008328B0; Construct remains explicit
static_assert(sizeof(global_file_thread)==0x28,"original global extent");
}
