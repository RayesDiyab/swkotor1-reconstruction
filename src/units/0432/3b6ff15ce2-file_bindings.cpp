#include "897ca13cc0-internal.h"
#include "5072eb8c01-directory_data.h"
#include "90c0a5b35a-allocator_live.h"
namespace tlk_file {
namespace {
// One constant-initialized descriptor, actual File4/Internal20 typed targets.
const Bindings file_descriptor={
 &file_internal::exofile_construct_005e68d0,
 &file_internal::exofile_seek_beginning_005e6ad0,
 &file_internal::exofile_read_005e6960,
 &file_internal::internal_destroy_005eb900,
 &live_free,c2da_directory_literal_0073d5b0
};
}
const Bindings* bindings=&file_descriptor;
}
// Engine and FILE pointer cells remain unresolved until their genuine owners
// and complete publication contracts are composed; no null/success stand-ins.
